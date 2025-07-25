# BlotAddons.cmake
#
# Usage:
#   set(APP_MANIFEST "${CMAKE_CURRENT_SOURCE_DIR}/apps/example_app/app.json")
#   include(cmake/BlotAddons.cmake)
#   blot_load_addons(${APP_MANIFEST} BLOT_ADDON_LIBS)
#   target_link_libraries(example_app PRIVATE ${BLOT_ADDON_LIBS})
#
# Requires CMake >= 3.19 for file(JSON ...)

function(blot_load_addons MANIFEST_PATH OUT_VAR)
    if(NOT EXISTS ${MANIFEST_PATH})
        message(FATAL_ERROR "Manifest file not found: ${MANIFEST_PATH}")
    endif()

    if(${CMAKE_VERSION} VERSION_LESS "3.19")
        message(FATAL_ERROR "BlotAddons.cmake requires CMake 3.19 or newer for JSON parsing, found ${CMAKE_VERSION}.")
    endif()

    message(STATUS "### Running CMake ${CMAKE_VERSION} (${CMAKE_COMMAND})")
    # Read the manifest JSON text into a variable and parse it with
    # string(JSON) (file(JSON ...) does not exist; string(JSON) was added
    # in CMake 3.19).
    file(READ "${MANIFEST_PATH}" _manifest)

    # Initialize global list of processed addons if not already set
    if(NOT DEFINED BLOT_PROCESSED_ADDONS)
        set(BLOT_PROCESSED_ADDONS "" CACHE INTERNAL "List of processed addons")
    endif()

    # Get length of dependencies array
    string(JSON _depsLen LENGTH "${_manifest}" dependencies)

    if(_depsLen EQUAL 0)
        message(STATUS "[BlotAddons] No dependencies listed in manifest ${MANIFEST_PATH}")
        set(${OUT_VAR} "" PARENT_SCOPE)
        return()
    endif()

    set(_addon_libs "")
    math(EXPR _lastIdx "${_depsLen}-1")
    foreach(idx RANGE 0 ${_lastIdx})
        string(JSON depName   GET "${_manifest}" dependencies ${idx} name)
        string(JSON depUrl    GET "${_manifest}" dependencies ${idx} url)
        string(JSON depBranch GET "${_manifest}" dependencies ${idx} branch)

        if(depName STREQUAL "")
            message(WARNING "[BlotAddons] Dependency entry ${idx} missing 'name'; skipping")
            continue()
        endif()

        # Set the addon directory in the main addons folder
        set(ADDON_DIR "${CMAKE_SOURCE_DIR}/addons/${depName}")
        
        # Check if this package has already been processed
        list(FIND BLOT_PROCESSED_ADDONS ${depName} _found)
        if(NOT _found EQUAL -1)
            message(STATUS "[BlotAddons] Package ${depName} already processed, skipping")
            list(APPEND _addon_libs ${depName})
            continue()
        endif()
        
        # Check if addon already exists locally
        if(EXISTS "${ADDON_DIR}")
            message(STATUS "[BlotAddons] Using local addon: ${depName}")
            # Build addon in global namespace to avoid std namespace pollution
            add_subdirectory(${ADDON_DIR})
        else()
            message(STATUS "[BlotAddons] Cloning addon: ${depName}")
            CPMAddPackage(
                NAME ${depName}
                GIT_REPOSITORY ${depUrl}
                GIT_TAG ${depBranch}
                GIT_SHALLOW TRUE
                SOURCE_DIR ${ADDON_DIR}
                OPTIONS "ADD_SUBDIRECTORY"
            )
        endif()
        
        # Mark this addon as processed
        list(APPEND BLOT_PROCESSED_ADDONS ${depName})
        set(BLOT_PROCESSED_ADDONS ${BLOT_PROCESSED_ADDONS} CACHE INTERNAL "List of processed addons")

        list(APPEND _addon_libs ${depName})
    endforeach()

    list(REMOVE_DUPLICATES _addon_libs)
    set(${OUT_VAR} ${_addon_libs} PARENT_SCOPE)
endfunction() 
