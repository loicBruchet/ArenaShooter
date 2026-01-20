#include "NexronPrefab.h"

#include <Engine.h>
#include <Shapes.h>

#include "Framework/ResourceManager.h"

#include "Game/Scripts/Weapon/Weapon.h"
#include "Game/Scripts/Stats/HealthStat.h"
#include "Game/Scripts/Stats/ShieldStat.h"

#include "Game/Scripts/Enemy/NexronAI.h"
#include "Game/Tags.h"

NexronPrefab::NexronPrefab(gce::Scene& _scene, json* _properties) : Prefab(_scene, _properties) {
	gce::Geometry* GEO = RESOURCE_MANAGER.GetGeometry("Mesh_Ennemy_Nexron");

	gce::GameObject& object = m_root;
	object.SetTags(TAG_DEFAULT | TAG_ENTITY | TAG_ENEMY);
	object.SetName("Nexron");

	gce::SphereCollider& collider = *object.AddComponent<gce::SphereCollider>();
	collider.localRay = 0.3f;
	collider.localScale = 4.0f;
	collider.isTrigger = false;
	object.transform.SetLocalPosition({ 10.5f,1.5f,10.5f });

	gce::MeshRenderer& mesh = *object.AddComponent<gce::MeshRenderer>();
	mesh.SetGeometry(GEO);

	HealthStat& health = *object.AddScript<HealthStat>();
	health.Init(512);

	gce::GameObject& spawnpoint = gce::GameObject::Create(_scene);
	spawnpoint.SetName("WeaponSpawnpoint");
	spawnpoint.SetParent(object);
	spawnpoint.transform.SetWorldPosition(object.transform.GetWorldPosition());
	spawnpoint.transform.LocalTranslate({ 0.0f, -1.4f, 0.0f });

	Weapon& weapon = *object.AddScript<Weapon>();
	weapon.bulletPrefabName = "ExplosiveProjectilePrefab";
	weapon.spawnpoint = &spawnpoint;

	NexronAI& ai = *object.AddScript<NexronAI>();
}
NexronPrefab::~NexronPrefab() {}