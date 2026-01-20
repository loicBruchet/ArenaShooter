#include "GuidedShotgunProjectile.h"

#include "Game/Tags.h"
#include "Game/Scripts/Stats/HealthStat.h"

void GuidedShotgunProjectile::Start()  {
	Projectile::Start();
	m_collider = m_pOwner->GetComponent<gce::SphereCollider>();

	gce::GameObject& radiusVisualizer = gce::GameObject::Create(*gce::GameManager::GetActiveScene());
	radiusVisualizer.SetName("HomingRadiusVisualizer");
	radiusVisualizer.SetParent(*m_pOwner);
	radiusVisualizer.SetTags(TAG_OTHER);
	radiusVisualizer.transform.SetLocalPosition({ 0.0f, 0.0f, 0.0f });
	radiusVisualizer.transform.SetWorldScale({ m_homingRadius, m_homingRadius, m_homingRadius });

	//gce::MeshRenderer& meshRenderer = *radiusVisualizer.AddComponent<gce::MeshRenderer>();
	//meshRenderer.SetGeometry(gce::SHAPES.SPHERE);

	gce::SphereCollider& sphereCollider = *radiusVisualizer.AddComponent<gce::SphereCollider>();
	sphereCollider.localScale = 1.0f;
	sphereCollider.localRay = 0.5f;
	sphereCollider.isTrigger = true;

	sphereCollider.collisionEnterEvent.AddListener(&GuidedShotgunProjectile::OnCollisionEnterHoming,this);
}
void GuidedShotgunProjectile::Update()  {
	Projectile::Update();

	if (m_hasTarget == false)
		return;

	gce::Quaternion current_rotation = m_pOwner->transform.GetWorldRotation();

	gce::Quaternion target_rotation;
	target_rotation.LookAt((target->transform.GetWorldPosition() - m_pOwner->transform.GetWorldPosition()).SelfNormalize());

	float distance = (target->transform.GetWorldPosition() - m_pOwner->transform.GetWorldPosition()).Norm();

	float adjustement_speed = 1 / distance;

	current_rotation.Lerp(target_rotation,gce::GameManager::DeltaTime() * adjustement_speed * 75);
	m_pOwner->transform.SetWorldRotation(current_rotation);
}

void GuidedShotgunProjectile::Destroy()  {
	Projectile::Destroy();
	m_collider->collisionEnterEvent.ClearListeners();
}

void GuidedShotgunProjectile::OnCollisionEnterHoming(gce::GameObject* _other){
	if (_other->HasTag(TAG_ENEMY)) {
		target = _other;
		m_hasTarget = true;
	}
}
void GuidedShotgunProjectile::OnHit(gce::GameObject* _other) {
	if (_other->HasTag(TAG_ENEMY)) {
		HealthStat* health = _other->GetScript<HealthStat>();

		if (health == NULL)
			return;

		health->TakeDammage(10);
	}
}