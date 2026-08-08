#pragma once

#include <string>

namespace Translation {
	void Install();
	const char* Translate(const std::string& key);
}
