#include "Projectile.h"

#include "Game/Utils/ColliderUtils.h"
#include "Game/Tags.h"

void Projectile::Start() {
	m_collider = m_pOwner->GetComponent<gce::SphereCollider>();

	if (destroyOnHit) { m_timer->Start(); }

	m_collider->collisionEnterEvent.AddListener(&Projectile::OnCollisionEnter, this);
}

void Projectile::Update() {
	gce::Vector3f32 velocity;
	velocity = m_pOwner->transform.GetWorldForward() * speed * gce::GameManager::DeltaTime();
	m_pOwner->transform.LocalTranslate(velocity);
	if (!m_timer->IsCompleted() || !destroyOnLifetimeEnd)
		return;

	m_collider->collisionEnterEvent.ClearListeners();
	m_pOwner->Destroy();
}

void Projectile::OnCollisionEnter(gce::GameObject* _other) {
	if (IsTrigger(_other))
		return;
	if(_other->HasTag(TAG_OTHER))
		return;

	OnHit(_other);

	if (!destroyOnHit)
		return;

	delete m_timer;

	m_collider->collisionEnterEvent.ClearListeners();
	m_pOwner->Destroy();
}