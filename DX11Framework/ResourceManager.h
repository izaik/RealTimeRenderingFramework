#pragma once

#include <unordered_map>
#include "resource.h"

class ResourceManager {
	std::unordered_map<std::string, std::shared_ptr<Resource>> ResourceCache; //Using strings as key for now, maybe replace with string hashes?

public:
	template<typename T = Resource, typename... TArgs>
	std::shared_ptr<T> CacheResource(std::string ResourceName, TArgs&& ...args) {
		if (ResourceCache.contains(ResourceName)) {
			std::shared_ptr<T> Reference = ResourceCache[ResourceName];

			return Reference;
		}
		else {
			auto NewResource = std::make_shared<T>(std::forward<TArgs>args...); //calls constructor of Resource type and makes it a shared pointer
			ResourceCache[ResourceName] = NewResource; //add new resource to map

			std::shared_ptr<T> Reference = ResourceCache[ResourceName]; //now make a reference to it

			return Reference;
		}
	}
};