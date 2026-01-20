#include "SilverAI.h"

#include "Game/Tags.h"
#include "Game/Scripts/Stats/ShieldStat.h"

void SilverAI::Start() {
	AIController::Start();

	m_agent = m_pOwner->GetScript<Agent>();
	m_agent->movementSpeed = 20.0f;
	m_agent->rotationSpeed = 10.0f;

	sightRadius = 100.0f;
	loseSightTime = 5.0f;

	m_silverCollider = m_pOwner->GetComponent<gce::SphereCollider>();
	m_silverCollider->collisionEnterEvent.AddListener(&SilverAI::OnCollisionWithPlayer, this);
}

void SilverAI::Destroy() {
	AIController::Destroy();
	m_silverCollider->collisionEnterEvent.ClearListeners();
	m_silverCollider = nullptr;
	delete m_delayTimer;
	m_delayTimer = nullptr;
	m_agent = nullptr;
}

void SilverAI::OnIdle(gce::GameObject* _player) {
	// Silver AI does nothing while idle
	m_waiting = false; // Reset waiting state
}

void SilverAI::OnAttack(gce::GameObject* _player) {
	// Silver AI logic for attacking the player

	if (_player == nullptr)
		return;

	if(!TurnTowardsTarget(_player))
		return;

	if(!m_waiting) {
		m_delayTimer->Restart();
		m_waiting = true;
		return;
	}

	if(!m_delayTimer->IsCompleted())
		return;

	//if (!m_attackCooldownTimer->IsCompleted())
	//	return;

	gce::Vector3f32 target = _player->transform.GetWorldPosition();
	m_agent->GoTo(target);
}

bool SilverAI::TurnTowardsTarget(gce::GameObject* _target) {
	gce::Vector3f32 direction = _target->transform.GetWorldPosition() - m_pOwner->transform.GetWorldPosition();
	direction.SelfNormalize();

	gce::Quaternion current_rotation = m_pOwner->transform.GetWorldRotation();
	gce::Quaternion target_rotation = gce::Quaternion::RotationAxis({ 0.0f, 1.0f, 0.0f }, atan2f(direction.x, direction.z));

	current_rotation.Lerp(target_rotation, 16 * gce::GameManager::DeltaTime());
	m_pOwner->transform.SetWorldRotation(current_rotation);

	// return true if facing the target
	return current_rotation.Dot(target_rotation) > 0.999f;
}
void SilverAI::OnCollisionWithPlayer(gce::GameObject* _player) {
	if (_player->HasTag(TAG_PLAYER)) {
		_player->GetScript<ShieldStat>()->TakeDammage(1);

		m_silverCollider->collisionEnterEvent.ClearListeners();	
		m_pOwner->Destroy();
	}
}