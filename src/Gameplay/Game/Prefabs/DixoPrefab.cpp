#include "DixoPrefab.h"

#include <Engine.h>
#include <Shapes.h>

#include "Framework/ResourceManager.h"

#include "Game/Scripts/Weapon/Weapon.h"
#include "Game/Scripts/Stats/HealthStat.h"
#include "Game/Scripts/Stats/ShieldStat.h"

#include "Game/Scripts/Enemy/DixoAI.h"
#include "Game/Tags.h"

DixoPrefab::DixoPrefab(gce::Scene& _scene, json* _properties) : Prefab(_scene, _properties) {
	gce::Geometry* DIXO_GEO = RESOURCE_MANAGER.GetGeometry("Mesh_Ennemy_Armox");

	gce::GameObject& dixoObject = m_root;
	dixoObject.SetTags(TAG_DEFAULT | TAG_ENTITY | TAG_ENEMY);

	gce::SphereCollider& dixoObjectCollider = *dixoObject.AddComponent<gce::SphereCollider>();
	dixoObjectCollider.localRay = 0.3f;
	dixoObjectCollider.localScale = 4.0f;
	dixoObjectCollider.isTrigger = false;
	dixoObject.transform.SetLocalPosition({ 10.5f,1.5f,10.5f });

	gce::MeshRenderer& dixoMeshRenderer = *dixoObject.AddComponent<gce::MeshRenderer>();
	dixoMeshRenderer.SetGeometry(DIXO_GEO);
	dixoObject.SetName("Dixo");

	HealthStat& dixoHealth = *dixoObject.AddScript<HealthStat>();
	dixoHealth.Init(100);

	gce::GameObject& weaponSpawnpoint = gce::GameObject::Create(_scene);
	weaponSpawnpoint.SetName("WeaponSpawnpoint");
	weaponSpawnpoint.SetParent(dixoObject);
	weaponSpawnpoint.transform.SetWorldPosition(dixoObject.transform.GetWorldPosition());
	weaponSpawnpoint.transform.LocalTranslate({ 0.0f, -1.4f, 0.0f });

	Weapon& dixoWeapon = *dixoObject.AddScript<Weapon>();
	dixoWeapon.bulletPrefabName = "ExplosiveProjectilePrefab";
	dixoWeapon.spawnpoint = &weaponSpawnpoint;

	DixoAI& dixoAI = *dixoObject.AddScript<DixoAI>();
}
DixoPrefab::~DixoPrefab() {}