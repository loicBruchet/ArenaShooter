#include "PrefabRegistry.h"

#include <Engine.h>

PrefabRegistry* PrefabRegistry::instance = nullptr;

PrefabRegistry& PrefabRegistry::Get() {
	if(instance == nullptr) {
		instance = new PrefabRegistry();
	}
	return *instance;
}

void PrefabRegistry::RegisterPrefab(const std::string& _type, PrefabFactory _factory) {
	prefab_factories[_type] = _factory;
}

Prefab* PrefabRegistry::CreatePrefab(const std::string& _type, gce::Scene& _scene, json* _properties) {
	if (prefab_factories.find(_type) != prefab_factories.end()) {
		return prefab_factories[_type](_scene, _properties);
	}
	return nullptr;
}

std::vector<std::string> PrefabRegistry::GetPrefabTypes() {
	std::vector<std::string> types;
	for (auto& pair : prefab_factories) {
		types.push_back(pair.first);
	}
	return types;
}