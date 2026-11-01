#pragma once

#include <unordered_map>
#include "resource.h"

class ResourceManager {
	std::unordered_map<std::string, std::weak_ptr<Resource>> ResourceCache;
};