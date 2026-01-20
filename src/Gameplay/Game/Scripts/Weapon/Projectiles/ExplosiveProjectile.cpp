#include "ExplosiveProjectile.h"

#include "Game/Tags.h"
#include "Game/Scripts/Stats/ShieldStat.h"

#include "Game/Scripts/Player/PlayerCamera.h"

#include <Audio.h>
#include "Framework/ResourceManager.h"

void ExplosiveProjectile::Start() {
	Projectile::Start();

	destroyOnHit = false;
	destroyOnLifetimeEnd = false;

	m_exploding = false;

	m_delay->Reset();

	m_delay->OnCompleted.AddListener(&ExplosiveProjectile::OnStartExplosion, this);
}

void ExplosiveProjectile::Update() {
	if (!m_exploding) {
		Projectile::Update();
		m_pOwner->transform.WorldTranslate(gce::Vector3f32(0.0f, -gce::GameManager::DeltaTime() * m_time * 50, 0.0f));
		m_time += gce::GameManager::DeltaTime();
	}
	else {
		if (m_target != nullptr) {
			if (!m_hit) {
				if (m_target->HasTag(TAG_PLAYER)) {
					ShieldStat* shield = m_target->GetScript<ShieldStat>();

					if (shield != nullptr) {
						shield->TakeDammage(2);
						m_hit = true;
					}
				}
			}
		}

		if (!m_delay->IsCompleted()) {
			return;
		}

		if (!m_exploded) {
			gce::GameObject* camera = gce::GameManager::GetActiveScene()->GetObjectByName("MainCamera");
			if (camera != nullptr) {
				camera->GetScript<PlayerCamera>()->CameraShake(0.35f, 0.5f);
			}
			m_exploded = true;
		}

		gce::Vector3f32 currentScale = m_pOwner->transform.GetWorldScale();
		currentScale += gce::Vector3f32(15.0f, 15.0f, 15.0f) * gce::GameManager::DeltaTime();
		m_pOwner->transform.SetWorldScale(currentScale);
		if (currentScale.x >= 10.0f) {
			delete m_timer;
			delete m_delay;
			m_collider->collisionEnterEvent.ClearListeners();
			m_pOwner->Destroy();
		}
	}
}

void ExplosiveProjectile::OnHit(gce::GameObject* _other) {
	m_exploding = true;
	m_delay->Start();

	if (_other->HasTag(TAG_PLAYER))
		m_target = _other;
}

void ExplosiveProjectile::OnStartExplosion() {
	gce::AudioUse::Play(RESOURCE_MANAGER.GetSound("ExplosionSound"), false, m_pOwner->transform.GetWorldPosition());
	gce::AudioUse::SetVolumeOfSound("ExplosionSound", 25);
}