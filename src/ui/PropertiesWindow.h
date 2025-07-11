#pragma once

#include "Window.h"
#include <imgui.h>
#include <memory>

// Forward declarations
class ECSManager;

namespace blot {

class PropertiesWindow : public Window {
public:
    PropertiesWindow(const std::string& title = "Properties", Flags flags = Flags::None);
    virtual ~PropertiesWindow() = default;

    // Properties management
    void setECSManager(std::shared_ptr<ECSManager> ecs);
    void setSelectedEntity(entt::entity entity);
    entt::entity getSelectedEntity() const;
    
    // Callbacks
    void setOnEntitySelected(std::function<void(entt::entity)> callback);
    void setOnPropertyChanged(std::function<void(entt::entity, const std::string&, const std::string&)> callback);

    // Rendering
    virtual void render() override;

    // Window interface overrides
    void show() override;
    void hide() override;
    void close() override;
    void toggle() override;
    bool isOpen() const override;
    bool isVisible() const override;
    bool isFocused() const override;
    bool isHovered() const override;
    bool isDragging() const override;
    bool isResizing() const override;
    void setPosition(const ImVec2&) override;
    void setSize(const ImVec2&) override;
    void setMinSize(const ImVec2&) override;
    void setMaxSize(const ImVec2&) override;
    void setFlags(Window::Flags) override;
    int getFlags() const override;
    std::string getTitle() const override;

private:
    std::shared_ptr<ECSManager> m_ecs;
    entt::entity m_selectedEntity = entt::null;
    
    // Callbacks
    std::function<void(entt::entity)> m_onEntitySelected;
    std::function<void(entt::entity, const std::string&, const std::string&)> m_onPropertyChanged;
    
    // Helper methods
    void renderEntityList();
    void renderTransformProperties();
    void renderShapeProperties();
    void renderStyleProperties();
    void renderColorPicker(const char* label, ImVec4& color);
    void renderVector2Editor(const char* label, float& x, float& y);
    void renderFloatEditor(const char* label, float& value, float min = 0.0f, float max = 100.0f);
    void renderIntEditor(const char* label, int& value, int min = 0, int max = 100);
};

} // namespace blot 