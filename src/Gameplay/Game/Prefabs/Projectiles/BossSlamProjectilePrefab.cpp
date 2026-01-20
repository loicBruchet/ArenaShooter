#include "BossSlamProjectilePrefab.h"

#include <Engine.h>
#include <Shapes.h>

#include "Game/Scripts/Weapon/Projectiles/BossSlamProjectile.h"

BossSlamProjectilePrefab::BossSlamProjectilePrefab(gce::Scene& _scene, json* _properties) : Prefab(_scene, _properties) {
	gce::GameObject& object = m_root;
	object.SetName("BossSlamProjectile");
	object.transform.SetLocalScale({ 0.2f,0.01f,0.2f });
	gce::SphereCollider& collider = *object.AddComponent<gce::SphereCollider>();
	collider.localScale = 1.25f;
	collider.localRay = 0.5f;
	collider.m_localOffSet = { 0.0f, 0.0f, 0.0f };
	collider.isTrigger = true;
	gce::MeshRenderer& mesh = *object.AddComponent<gce::MeshRenderer>();
	mesh.SetGeometry(gce::SHAPES.DONUT);
	mesh.SetCustomPso(gce::GameManager::GetUnlitPso());
	//gce::Light& light = *object.AddComponent<gce::Light>();
	//light.DefaultPointLight();
	BossSlamProjectile& script = *object.AddScript<BossSlamProjectile>();
}
BossSlamProjectilePrefab::~BossSlamProjectilePrefab() {}