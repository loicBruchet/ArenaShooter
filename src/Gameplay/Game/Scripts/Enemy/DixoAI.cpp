#include "DixoAI.h"

#include <Audio.h>
#include "Framework/ResourceManager.h"

void DixoAI::Start() {
	AIController::Start();

	sightRadius = 80.0f;

	m_weapon = m_pOwner->GetScript<Weapon>();

	m_weapon->maxAmmo = 6;
	m_weapon->currentAmmo = 6;
	m_weapon->fireRate = 1.0f;

	m_weapon->OnShoot.AddListener(&DixoAI::OnShoot, this);
}

void DixoAI::OnIdle(gce::GameObject* _player) {
	// Do nothing
}
void DixoAI::OnAttack(gce::GameObject* _player) {
	TurnTowardsTarget(_player);
	if (m_weapon->currentAmmo <= 0) {
		m_weapon->TryReload();
	}
	m_weapon->TryShoot();
}

bool DixoAI::TurnTowardsTarget(gce::GameObject* _target) {
	gce::Vector3f32 direction = _target->transform.GetWorldPosition() - m_pOwner->transform.GetWorldPosition();
	direction.SelfNormalize();

	gce::Quaternion current_rotation = m_pOwner->transform.GetWorldRotation();
	gce::Quaternion target_rotation = gce::Quaternion::RotationAxis({ 0.0f, 1.0f, 0.0f }, atan2f(direction.x, direction.z));

	current_rotation.Lerp(target_rotation, 16 * gce::GameManager::DeltaTime());
	m_pOwner->transform.SetWorldRotation(current_rotation);

	// return true if facing the target
	return current_rotation.Dot(target_rotation) > 0.999f;
}

void DixoAI::OnShoot() {
	gce::AudioUse::Play(RESOURCE_MANAGER.GetSound("ShootGrenade"), false, m_pOwner->transform.GetWorldPosition());
	gce::AudioUse::SetVolumeOfSound("ShootGrenade", 85);
}