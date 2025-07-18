#pragma once

#include "core/AddonBase.h"
#include <memory>
#include <string>

// Forward declaration
class TextEditor;

class bxCodeEditor : public blot::AddonBase {
  public:
	bxCodeEditor();
	~bxCodeEditor();
	// Inherit getName() from AddonBase which already returns the addon name set
	// via constructor.
	bool init() override { return true; }
	void setup() override {}
	void update(float) override {}
	void draw() override {}
	void cleanup() override {}

	void loadDefaultTemplate();
	std::string getCode() const;
	void setCode(const std::string &code);

  private:
	std::unique_ptr<TextEditor> m_editor;
	std::string m_currentCode;
};
