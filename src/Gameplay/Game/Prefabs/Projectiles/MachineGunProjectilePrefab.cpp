#include "MachineGunProjectilePrefab.h"

#include <Engine.h>
#include <Shapes.h>

#include "Game/Scripts/Weapon/Projectiles/MachineGunProjectile.h"
MachineGunProjectilePrefab::MachineGunProjectilePrefab(gce::Scene& _scene, json* _properties) : Prefab(_scene, _properties) {
	gce::GameObject& object = m_root;
	object.SetName("Bullet");
	object.transform.SetLocalScale({ 0.1f,0.1f,0.1f });
	gce::SphereCollider& collider = *object.AddComponent<gce::SphereCollider>();
	collider.localScale = 1.0f;
	collider.m_localOffSet = { 0.0f, 0.0f, 0.0f };
	collider.isTrigger = true;
	gce::MeshRenderer& mesh = *object.AddComponent<gce::MeshRenderer>();
	mesh.SetGeometry(gce::SHAPES.SPHERE);
	mesh.SetCustomPso(gce::GameManager::GetUnlitPso());
	//gce::Light& light = *object.AddComponent<gce::Light>();
	//light.DefaultPointLight();
	MachineGunProjectile& script = *object.AddScript<MachineGunProjectile>();
}
MachineGunProjectilePrefab::~MachineGunProjectilePrefab() {}