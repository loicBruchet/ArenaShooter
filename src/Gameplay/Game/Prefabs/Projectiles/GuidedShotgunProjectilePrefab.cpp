#include "GuidedShotgunProjectilePrefab.h"
#include <Engine.h>
#include <Shapes.h>

#include "Game/Scripts/Weapon/Projectiles/GuidedShotgunProjectile.h"

GuidedShotgunProjectilePrefab::GuidedShotgunProjectilePrefab(gce::Scene& _scene, json* _properties) : Prefab(_scene, _properties)
{
	gce::GameObject& homingObject = m_root;
	homingObject.SetName("HomingBullet");
	homingObject.transform.SetLocalScale({ 0.2f,0.2f,0.2f });
	gce::SphereCollider& homingObjectCollider = *homingObject.AddComponent<gce::SphereCollider>();
	homingObjectCollider.localScale = 1.0f;
	homingObjectCollider.m_localOffSet = { 0.0f, 0.0f, 0.0f };
	homingObjectCollider.isTrigger = true;
	gce::MeshRenderer& homingObjectMesh = *homingObject.AddComponent<gce::MeshRenderer>();
	homingObjectMesh.SetGeometry(gce::SHAPES.SPHERE);
	homingObjectMesh.SetCustomPso(gce::GameManager::GetUnlitPso());
	/*gce::Light& hommingLight = *homingObject.AddComponent<gce::Light>();
	hommingLight.DefaultPointLight();*/

	 GuidedShotgunProjectile& hommingProjectileScript = *homingObject.AddScript<GuidedShotgunProjectile>();
}
GuidedShotgunProjectilePrefab::~GuidedShotgunProjectilePrefab() {}