#include "LogWindow.h"
#include <imgui.h>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>

namespace blot {

LogWindow::LogWindow(const std::string& title, Flags flags)
    : Window(title, flags) {
    // Set initial window size
    m_state.size = ImVec2(500, 300);
    m_state.position = ImVec2(100, 100);
    
    // Add initial log entry
    addInfo("Log window initialized.");
}

// Window interface implementations
void LogWindow::show() { m_state.isOpen = true; }
void LogWindow::hide() { m_state.isOpen = false; }
void LogWindow::close() { m_state.isOpen = false; }
void LogWindow::toggle() { m_state.isOpen = !m_state.isOpen; }
bool LogWindow::isOpen() const { return m_state.isOpen; }
bool LogWindow::isVisible() const { return m_state.isOpen; }
bool LogWindow::isFocused() const { return ImGui::IsWindowFocused(); }
bool LogWindow::isHovered() const { return ImGui::IsWindowHovered(); }
bool LogWindow::isDragging() const { return false; }
bool LogWindow::isResizing() const { return false; }
void LogWindow::setPosition(const ImVec2& pos) { m_state.position = pos; }
void LogWindow::setSize(const ImVec2& size) { m_state.size = size; }
void LogWindow::setMinSize(const ImVec2& minSize) { /* TODO: Implement if needed */ }
void LogWindow::setMaxSize(const ImVec2& maxSize) { /* TODO: Implement if needed */ }
void LogWindow::setFlags(Window::Flags flags) { m_state.flags = static_cast<int>(flags); }
int LogWindow::getFlags() const { return m_state.flags; }
std::string LogWindow::getTitle() const { return m_title; }

void LogWindow::render() {
    if (ImGui::Begin(m_title.c_str(), &m_state.isOpen, m_state.flags)) {
        renderFilterControls();
        renderLogEntries();
    }
    ImGui::End();
}

void LogWindow::renderFilterControls() {
    ImGui::Text("Filter:");
    ImGui::SameLine();
    ImGui::Checkbox("Debug", &m_showDebug);
    ImGui::SameLine();
    ImGui::Checkbox("Info", &m_showInfo);
    ImGui::SameLine();
    ImGui::Checkbox("Warning", &m_showWarning);
    ImGui::SameLine();
    ImGui::Checkbox("Error", &m_showError);
    ImGui::SameLine();
    if (ImGui::Button("Clear")) {
        clearLog();
    }
    ImGui::Separator();
}

void LogWindow::renderLogEntries() {
    // Create a child window for the log entries
    ImGui::BeginChild("LogEntries", ImVec2(0, 0), true);
    
    std::lock_guard<std::mutex> lock(m_logMutex);
    
    for (const auto& entry : m_logEntries) {
        // Check if this log level should be shown
        bool shouldShow = false;
        switch (entry.level) {
            case LogLevel::Debug: shouldShow = m_showDebug; break;
            case LogLevel::Info: shouldShow = m_showInfo; break;
            case LogLevel::Warning: shouldShow = m_showWarning; break;
            case LogLevel::Error: shouldShow = m_showError; break;
        }
        
        if (!shouldShow) continue;
        
        // Set color based on log level
        ImGui::PushStyleColor(ImGuiCol_Text, getLogColor(entry.level));
        
        // Display timestamp and level
        std::string header = "[" + entry.timestamp + "] [" + getLogLevelString(entry.level) + "] ";
        ImGui::TextUnformatted(header.c_str());
        ImGui::SameLine();
        
        // Display message
        ImGui::TextWrapped("%s", entry.message.c_str());
        
        ImGui::PopStyleColor();
    }
    
    if (m_scrollToBottom) {
        ImGui::SetScrollHereY(1.0f);
        m_scrollToBottom = false;
    }
    
    ImGui::EndChild();
}

ImVec4 LogWindow::getLogColor(LogLevel level) {
    switch (level) {
        case LogLevel::Debug: return ImVec4(0.5f, 0.5f, 0.5f, 1.0f);   // Gray
        case LogLevel::Info: return ImVec4(1.0f, 1.0f, 1.0f, 1.0f);     // White
        case LogLevel::Warning: return ImVec4(1.0f, 1.0f, 0.0f, 1.0f);  // Yellow
        case LogLevel::Error: return ImVec4(1.0f, 0.0f, 0.0f, 1.0f);    // Red
        default: return ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    }
}

std::string LogWindow::getLogLevelString(LogLevel level) {
    switch (level) {
        case LogLevel::Debug: return "DEBUG";
        case LogLevel::Info: return "INFO";
        case LogLevel::Warning: return "WARN";
        case LogLevel::Error: return "ERROR";
        default: return "UNKNOWN";
    }
}

std::string LogWindow::getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) % 1000;
    
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_t), "%H:%M:%S");
    ss << '.' << std::setfill('0') << std::setw(3) << ms.count();
    return ss.str();
}

void LogWindow::addLog(LogLevel level, const std::string& message) {
    std::lock_guard<std::mutex> lock(m_logMutex);
    m_logEntries.emplace_back(level, message, getCurrentTimestamp());
    
    // Limit log entries to prevent memory issues
    if (m_logEntries.size() > 1000) {
        m_logEntries.erase(m_logEntries.begin());
    }
    
    m_scrollToBottom = true;
}

void LogWindow::addDebug(const std::string& message) {
    addLog(LogLevel::Debug, message);
}

void LogWindow::addInfo(const std::string& message) {
    addLog(LogLevel::Info, message);
}

void LogWindow::addWarning(const std::string& message) {
    addLog(LogLevel::Warning, message);
}

void LogWindow::addError(const std::string& message) {
    addLog(LogLevel::Error, message);
}

void LogWindow::clearLog() {
    std::lock_guard<std::mutex> lock(m_logMutex);
    m_logEntries.clear();
    addInfo("Log cleared.");
}

} // namespace blot 