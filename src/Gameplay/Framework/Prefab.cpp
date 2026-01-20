#include "Prefab.h"

#include "PrefabRegistry.h"

#include <Engine.h>

Prefab::Prefab(gce::Scene& _scene, json* _properties)
	: m_scene(_scene), m_root(gce::GameObject::Create(_scene)) {}
Prefab::~Prefab() {}

PrefabRegistry& Prefab::GetRegistry() {
	return PrefabRegistry::Get();
}

gce::GameObject& Prefab::GetRootObject() const {
	return m_root;
}