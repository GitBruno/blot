#pragma once

// Standard library
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

// Third-party
#ifdef _WIN32
#include <windows.h>
#endif
#include <blend2d.h>
#include <glad/gl.h>
#include <glm/glm.hpp>

// Project headers
#include "rendering/IRenderer.h"

class Blend2DRenderer : public IRenderer {
  public:
	Blend2DRenderer();
	~Blend2DRenderer() override;

	// IRenderer implementation
	bool initialize(int width, int height) override;
	void shutdown() override;
	void resize(int width, int height) override;

	void beginFrame() override;
	void endFrame() override;
	void clear(const glm::vec4 &color) override;

	void drawLine(float x1, float y1, float x2, float y2) override;
	void drawRect(float x, float y, float width, float height) override;
	void drawCircle(float x, float y, float radius) override;
	void drawEllipse(float x, float y, float width, float height) override;
	void drawTriangle(float x1, float y1, float x2, float y2, float x3,
					  float y3) override;
	void drawPolygon(const std::vector<glm::vec2> &points) override;

	void beginPath() override;
	void moveTo(float x, float y) override;
	void lineTo(float x, float y) override;
	void curveTo(float cx1, float cy1, float cx2, float cy2, float x,
				 float y) override;
	void closePath() override;
	void fill(const glm::vec4 &color) override;
	void stroke(const glm::vec4 &color, float width) override;

	void setFont(const std::string &fontPath, float size) override;
	void drawText(const std::string &text, float x, float y,
				  const glm::vec4 &color) override;
	glm::vec2 getTextBounds(const std::string &text) override;

	void pushMatrix() override;
	void popMatrix() override;
	void translate(float x, float y) override;
	void rotate(float angle) override;
	void scale(float sx, float sy) override;
	void resetMatrix() override;

	bool saveToFile(const std::string &filename) override;
	bool saveToMemory(std::vector<uint8_t> &data) override;

	RendererType getType() const override { return RendererType::Blend2D; }
	std::string getName() const override { return "Blend2D"; }
	bool isInitialized() const override { return m_initialized; }
	int getWidth() const override;
	int getHeight() const override;

	void noFill();
	void noStroke();

	// Comprehensive stroke options
	void setStrokeCap(BLStrokeCap cap);
	void setStrokeJoin(BLStrokeJoin join);
	void setStrokeMiterLimit(double limit);
	void setStrokeDashArray(const std::vector<double> &dashes);
	void setStrokeDashOffset(double offset);
	void setStrokeTransformOrder(BLStrokeTransformOrder order);
	void setStrokeOptions(const BLStrokeOptions &options);

	// Legacy method for compatibility
	void setStrokeDashPattern(const std::vector<double> &dashes,
							  double offset = 0.0);

	void setFillColor(const glm::vec4 &color) override;
	void setStrokeColor(const glm::vec4 &color) override;
	void setStrokeWidth(float width) override;
	void flush();

	// Add this public getter for the BLImage
	const BLImage &getImage() const { return m_image; }

	// New methods for direct window drawing
	uint8_t *getPixelBuffer() override;
	void present();

  private:
	void setupContext();
	void renderPath();
	void updateStrokeOptions();
	BLRgba32 convertColor(const glm::vec4 &color);
	BLPoint convertPoint(const glm::vec2 &point);

	bool m_initialized;
	int m_width, m_height;

	// Blend2D objects
	BLContext m_context;
	BLImage m_image;
	BLFont m_font;

	// Path rendering
	BLPath m_currentPath;
	bool m_pathOpen;

	// Matrix stack
	std::vector<BLMatrix2D> m_matrixStack;
	BLMatrix2D m_currentMatrix;

	// Rendering state
	glm::vec4 m_currentFillColor;
	glm::vec4 m_currentStrokeColor;
	float m_currentStrokeWidth;
	bool m_hasFill = true;
	bool m_hasStroke = true;

	// Comprehensive stroke options
	BLStrokeOptions m_strokeOptions;
	glm::vec4 m_fillColor = glm::vec4(1, 1, 1, 1);
	glm::vec4 m_strokeColor = glm::vec4(0, 0, 0, 1);
	float m_strokeWidth = 1.0f;

	std::vector<uint8_t> m_pixelBuffer;
	GLuint m_textureId = 0;
	// Modern OpenGL members
	GLuint m_vao = 0, m_vbo = 0;
	GLuint m_shaderProgram = 0;
};
