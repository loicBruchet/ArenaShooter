#include "BossSlamProjectile.h"

#include "Game/Tags.h"
#include "Game/Scripts/Stats/ShieldStat.h"

void BossSlamProjectile::Start() {
	Projectile::Start();

	destroyOnHit = false;
	destroyOnLifetimeEnd = false;

	m_exploding = false;
}

void BossSlamProjectile::Update() {
	gce::Vector3f32 currentScale = m_pOwner->transform.GetWorldScale();
	currentScale += gce::Vector3f32(15.0f, 2.0f, 15.0f) * gce::GameManager::DeltaTime() * 2.0f;
	m_pOwner->transform.SetWorldScale(currentScale);
	if (currentScale.x >= 25.0f) {
		delete m_timer;
		m_collider->collisionEnterEvent.ClearListeners();
		m_pOwner->Destroy();
	}
}

void BossSlamProjectile::OnHit(gce::GameObject* _other) {
	if (!_other->HasTag(TAG_PLAYER))
		return;

	ShieldStat* shield = _other->GetScript<ShieldStat>();

	if (shield == nullptr)
		return;

	shield->TakeDammage(5);
}