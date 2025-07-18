#include <memory>
#include "app.h"
#include "core/BlotEngine.h"
#include "core/core.h"

int main(int, char **) {
	auto app = std::make_unique<SampleMenubarApp>();
	blot::BlotEngine engine(std::move(app));
	engine.run();
	return 0;
}
