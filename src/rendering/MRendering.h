#pragma once

#include <entt/entt.hpp>
#include <memory>
#include <unordered_map>
#include "core/IManager.h"
#include "core/ISettings.h"
#include "core/canvas/Canvas.h"
#include "rendering/Graphics.h"
#include "rendering/IRenderer.h"

/**
 * The Rendering Manager is the sole owner/manager of all engine-level IRenderer
 * instances. All renderer creation, switching, and access should go through here.
 */
namespace blot {
class MRendering : public IManager, public ISettings {
  public:
	MRendering();
	~MRendering();
	void init() override {}
	void shutdown() override {}

	// Main renderer management
	void setMainRenderer(std::shared_ptr<IRenderer> renderer);
	std::shared_ptr<IRenderer> getMainRenderer() const;

	// Renderer management
	std::shared_ptr<IRenderer> getRenderer(entt::entity entity);
	std::shared_ptr<IRenderer> createRenderer(entt::entity entity,
											  RendererType type, int width,
											  int height);
	void destroyRenderer(entt::entity entity);

	// Canvas management
	std::unique_ptr<Canvas> *getCanvas(entt::entity entity);
	void addCanvas(entt::entity entity, std::unique_ptr<Canvas> canvas);
	void removeCanvas(entt::entity entity);
	std::unordered_map<entt::entity, std::unique_ptr<Canvas>> &getCanvases() {
		return m_canvases;
	}

	// Graphics management
	std::shared_ptr<Graphics> getGraphics(entt::entity entity);
	void addGraphics(entt::entity entity, std::shared_ptr<Graphics> graphics);
	void removeGraphics(entt::entity entity);

	// Resource cleanup
	void cleanup();

	// ISettings interface
	json getSettings() const override;
	void setSettings(const json &settings) override;

  private:
	std::shared_ptr<IRenderer> m_mainRenderer;
	std::unordered_map<entt::entity, std::shared_ptr<IRenderer>> m_renderers;
	std::unordered_map<entt::entity, std::unique_ptr<Canvas>> m_canvases;
	std::unordered_map<entt::entity, std::shared_ptr<Graphics>> m_graphics;
};
} // namespace blot

// -----------------------------------------------------------------------------
// Helper functions formerly in Renderer.cpp

// Factory helper – remains a free function so callers can create a renderer
// without dealing with the registry directly.
std::shared_ptr<IRenderer> createRenderer(RendererType type);

// Convert a user-friendly string to a RendererType enum value.
RendererType getRendererTypeFromString(const std::string &name);

// Return the list of human-readable renderer names supported by the engine.
std::vector<std::string> getAvailableRendererNames();
