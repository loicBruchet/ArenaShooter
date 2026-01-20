#include "ChromePrefab.h"

#include <Engine.h>
#include <Shapes.h>

#include "Framework/ResourceManager.h"

#include "Game/Scripts/Weapon/Weapon.h"
#include "Game/Scripts/Stats/HealthStat.h"
#include "Game/Scripts/Stats/ShieldStat.h"

#include "Game/Scripts/Enemy/ChromeAI.h"

ChromePrefab::ChromePrefab(gce::Scene& _scene, json* _properties) :  Prefab(_scene, _properties) {
	gce::Geometry* CHROME_GEO = RESOURCE_MANAGER.GetGeometry("Mesh_Ennemy_Chrome");

	gce::GameObject& chromeObject = m_root;

	gce::SphereCollider& chromeObjectCollider = *chromeObject.AddComponent<gce::SphereCollider>();
	chromeObjectCollider.localRay = 0.3f;
	chromeObjectCollider.localScale = 4.0f;
	chromeObjectCollider.isTrigger = false;
	chromeObject.transform.SetLocalPosition({ 10.5f,1.5f,10.5f });

	gce::MeshRenderer& chromeMeshRenderer = *chromeObject.AddComponent<gce::MeshRenderer>();
	chromeMeshRenderer.SetGeometry(CHROME_GEO);
	chromeObject.SetName("Chrome");
	chromeObject.SetTags(TAG_ENEMY);

	HealthStat& chromeHealth = *chromeObject.AddScript<HealthStat>();
	chromeHealth.Init(10);

	ShieldStat& chromeShield = *chromeObject.AddScript<ShieldStat>();
	chromeShield.Init(5);

	gce::GameObject& weaponSpawnpoint = gce::GameObject::Create(_scene);
	weaponSpawnpoint.SetName("WeaponSpawnpoint");
	weaponSpawnpoint.SetParent(chromeObject);
	weaponSpawnpoint.transform.SetWorldPosition(chromeObject.transform.GetWorldPosition());
	weaponSpawnpoint.transform.LocalTranslate({ 0.0f, -1.4f, 0.0f });


	Weapon& chromeWeapon = *chromeObject.AddScript<Weapon>();
	chromeWeapon.spawnpoint = &weaponSpawnpoint;
	chromeWeapon.bulletPrefabName = "MachineGunProjectilePrefab";
	ChromeAI& chromeAI = *chromeObject.AddScript<ChromeAI>();
}
ChromePrefab::~ChromePrefab() {}