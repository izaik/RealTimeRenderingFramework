#pragma once

#include <unordered_map>
#include "resource.h"

class ResourceManager {
	std::unordered_map<size_t, std::shared_ptr<Resource>> ResourceCache; //Using strings as key for now, maybe replace with string hashes?

	std::hash<std::string> hasher;
public:
	template<typename T = Resource, typename... TArgs>
	std::shared_ptr<T> GetResource(std::string ResourcePath, TArgs&& ...args) {
		size_t id = hasher(ResourcePath);

		auto it = ResourceCache.find(id);
		if (it != ResourceCache.end()) {
			return it->second; //return second from it because that is the value for the k : v pair
		}

		else {
			auto NewResource = std::make_shared<T>(std::forward<TArgs>args...); //calls constructor of Resource type and makes it a shared pointer
			ResourceCache[id] = NewResource; //add new resource to map

			return ResourceCache[id];
		}
	}
};