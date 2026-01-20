#include "NexronAI.h"

#include "Framework/Prefab.h"
#include "Framework/PrefabRegistry.h"
#include "Game/Scripts/Player/PlayerCamera.h"
#include "Game/Scripts/Stats/ShieldStat.h"
#include "Game/Tags.h"

#include <cmath>
#include <cstdlib>
#include "Game/Levels.h"

#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

int NexronAI::RandomRange(int min, int max) {
	return min + (rand() % (max - min + 1));
}

NexronState NexronAI::GetRandomAttackState() {
	return RandomRange(0, 1) == 0
		? NexronState::SlamAttack
		: NexronState::DashAttack;
}

void NexronAI::Start() {
	AIController::Start();

	m_dammageSphere = &gce::GameObject::Create(*gce::GameManager::GetActiveScene());
	std::string* name = new std::string(std::string(m_pOwner->GetName()) + "_DammageSphere");
	m_dammageSphere->SetName(name->c_str());
	m_dammageSphere->SetParent(*m_pOwner);
	m_dammageSphere->transform.SetLocalPosition({ 0.0f, 0.0f, 0.0f });
	m_dammageSphere->transform.SetLocalScale({ 5.0f, 5.0f, 5.0f });

	m_dammageSphereCollider = m_dammageSphere->AddComponent<gce::SphereCollider>();
	m_dammageSphereCollider->localScale = 1.0f;
	m_dammageSphereCollider->localRay = 0.5f;
	m_dammageSphereCollider->isTrigger = true;
	m_dammageSphereCollider->collisionEnterEvent.AddListener(&NexronAI::OnDammageSphereEnter, this);

	m_initialPosition = m_pOwner->transform.GetWorldPosition();

	sightRadius = 100.0f;

	m_currentState = NexronState::Idle;

	m_healthStat = m_pOwner->GetScript<HealthStat>();
	m_healthStat->OnDeath.AddListener(&NexronAI::OnDeath, this);
}

void NexronAI::OnIdle(gce::GameObject* _player) {
	if (_player == nullptr)
		return;

	ComeBackToPosition();
}

void NexronAI::OnAttack(gce::GameObject* _player) {
	if (_player == nullptr)
		return;

	float dt = gce::GameManager::DeltaTime();

	if (!m_fightStarted) {
		m_fightStarted = true;
		m_waitingFightStart = true;
		m_fightStartTimer = 0.0f;
		return;
	}

	if (m_waitingFightStart) {
		m_fightStartTimer += dt;
		if (m_fightStartTimer >= 3.0f) {
			m_waitingFightStart = false;
			StartNewWaveGroup();
		}
		return;
	}

	switch (m_currentState) {
	case NexronState::SlamAttack:
		SlamAttack(_player);
		break;

	case NexronState::DashAttack:
		DashAttack(_player);
		break;

	case NexronState::Idle:
		ComeBackToPosition();
		
		float distanceToStart = (m_pOwner->transform.GetWorldPosition() - m_initialPosition).Norm();
		gce::Vector3f32 currentScale = m_pOwner->transform.GetLocalScale();
		float scaleDistance = (currentScale - gce::Vector3f32(1.0f, 1.0f, 1.0f)).Norm();
		
		if (distanceToStart < 0.5f && scaleDistance < 0.1f) {
			if (m_pendingNewAttack) {
				m_pendingNewAttack = false;
				StartNewAttackSequence();
			}
			else {
				m_idleBetweenWavesTimer += dt;

				if (m_idleBetweenWavesTimer >= m_idleBetweenWavesDuration) {
					StartNewWaveGroup();
				}
			}
		}
		break;
	}
}

void NexronAI::StartNewWaveGroup() {
	m_wavesRemaining = RandomRange(1, 3);
	StartNewAttackSequence();
}

void NexronAI::StartNewAttackSequence() {
	m_attacksRemainingInSequence = RandomRange(1, 5);
	m_currentState = GetRandomAttackState();
}

void NexronAI::SlamAttack(gce::GameObject* _player) {
	static float ground_pos = m_initialPosition.y;
	static float v_velocity = 0.0f;

	if (m_pOwner->transform.GetWorldPosition().y <= ground_pos) {
		v_velocity = 2.0f;

		Prefab* prefab = PREFAB_REGISTRY.CreatePrefab(
			"BossSlamProjectilePrefab",
			*gce::GameManager::GetActiveScene()
		);

		gce::GameObject& projectile = prefab->GetRootObject();
		projectile.transform.SetWorldPosition(
			m_pOwner->transform.GetWorldPosition() + gce::Vector3f32(0.0f, -4.2f, 0.0f)
		);

		// Camera shake
		gce::GameObject* cameraObject =
			gce::GameManager::GetActiveScene()->GetObjectByName("MainCamera");

		if (cameraObject) {
			PlayerCamera* cam = cameraObject->GetScript<PlayerCamera>();
			if (cam) cam->CameraShake(0.5f, 0.5f);
		}

		// Sequence logic
		m_attacksRemainingInSequence--;
		if (m_attacksRemainingInSequence <= 0) {
			m_wavesRemaining--;
			if (m_wavesRemaining > 0) {
				m_pendingNewAttack = true;
				m_currentState = NexronState::Idle;
				m_idleBetweenWavesTimer = 0.0f;
			}
			else {
				m_currentState = NexronState::Idle;
				m_idleBetweenWavesTimer = 0.0f;
			}
		}
	}
	else {
		v_velocity -= 0.5f * gce::GameManager::DeltaTime() * 8.0f;
	}

	m_pOwner->transform.WorldTranslate(
		gce::Vector3f32(0.0f, v_velocity * gce::GameManager::DeltaTime() * 4.0f, 0.0f)
	);

	float scale_y = 1.0f + (v_velocity * 0.1f);
	m_pOwner->transform.SetLocalScale({
		1.0f / std::sqrt(scale_y),
		scale_y,
		1.0f / std::sqrt(scale_y)
		});
}

void NexronAI::DashAttack(gce::GameObject* _player) {
	if (!m_dashInitialized) {
		m_dashStartPosition = m_pOwner->transform.GetWorldPosition();
		m_dashTargetPosition = _player->transform.GetWorldPosition();
		m_dashTargetPosition.y = m_dashStartPosition.y;

		gce::Vector3f32 dir = (m_dashTargetPosition - m_dashStartPosition).Normalize();
		float dist = (m_dashTargetPosition - m_dashStartPosition).Norm();

		if (dist > m_maxDashDistance)
			m_dashTargetPosition = m_dashStartPosition + dir * m_maxDashDistance;

		m_dashTargetPosition -= dir * m_dashStopDistance;

		m_dashProgress = 0.0f;
		m_dashInitialized = true;
	}

	m_dashProgress += gce::GameManager::DeltaTime() / m_dashDuration;
	m_dashProgress = std::min(m_dashProgress, 1.0f);

	float ease = m_dashProgress == 1.0f
		? 1.0f
		: 1.0f - std::pow(2.0f, -10.0f * m_dashProgress);

	m_pOwner->transform.SetWorldPosition(
		m_dashStartPosition +
		(m_dashTargetPosition - m_dashStartPosition) * ease
	);

	if (m_dashProgress >= 1.0f) {
		m_dashInitialized = false;
		m_attacksRemainingInSequence--;

		if (m_attacksRemainingInSequence > 0) {
			m_pendingNewAttack = true;
			m_currentState = NexronState::Idle;
			m_idleBetweenWavesTimer = 0.0f;
		}
		else {
			m_wavesRemaining--;
			if (m_wavesRemaining > 0) {
				m_pendingNewAttack = true;
				m_currentState = NexronState::Idle;
				m_idleBetweenWavesTimer = 0.0f;
			}
			else {
				m_currentState = NexronState::Idle;
				m_idleBetweenWavesTimer = 0.0f;
			}
		}
	}
}

void NexronAI::ComeBackToPosition() {
	m_pOwner->transform.SetWorldPosition(
		gce::Vector3f32::Lerp(
			m_pOwner->transform.GetWorldPosition(),
			m_initialPosition,
			0.01f
		)
	);
	
	m_pOwner->transform.SetLocalScale(
		gce::Vector3f32::Lerp(
			m_pOwner->transform.GetLocalScale(),
			gce::Vector3f32(1.0f, 1.0f, 1.0f),
			0.1f
		)
	);
}

void NexronAI::OnDammageSphereEnter(gce::GameObject* _other) {
	if (m_currentState != NexronState::DashAttack)
		return;

	if (!_other->HasTag(TAG_PLAYER))
		return;

	ShieldStat* shield = _other->GetScript<ShieldStat>();
	if (!shield)
		return;

	shield->TakeDammage(5);

	m_currentState = NexronState::Idle;
	m_idleBetweenWavesTimer = 0.0f;
}

void NexronAI::OnDeath(gce::GameObject* _gm) {
	delete m_loseSightTimer;

	gce::Scene& newScene = gce::Scene::Create();
	gce::GameManager::ChangeScene(&newScene);
	WinLevel(newScene);
}