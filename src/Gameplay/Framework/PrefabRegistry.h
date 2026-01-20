#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "Utils/Json.hpp"

#define PREFAB_REGISTRY PrefabRegistry::Get()

class Prefab;
namespace gce {
	class Scene;
}
//using PrefabFactory = Prefab * (*)(gce::Scene&);
using PrefabFactory = Prefab * (*)(gce::Scene&, json*);

class PrefabRegistry {
public:
	static PrefabRegistry& Get();

	void RegisterPrefab(const std::string& _type, PrefabFactory _factory);
	Prefab* CreatePrefab(const std::string& _type, gce::Scene& _scene, json* _properties = nullptr);
	std::vector<std::string> GetPrefabTypes();

protected:
	PrefabRegistry() = default;
	virtual ~PrefabRegistry() = default;
private:
	static PrefabRegistry* instance;

	std::unordered_map<std::string, PrefabFactory> prefab_factories;
};