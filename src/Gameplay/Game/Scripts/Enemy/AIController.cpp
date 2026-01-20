#include "AIController.h"

void AIController::Start() {
	m_player = gce::GameManager::GetObjectByName("Player");

	m_healthStat = m_pOwner->GetScript<HealthStat>();
	m_healthStat->OnTakeDammage.AddListener(&AIController::OnTakeDammage, this);

	m_radiusChecker = &gce::GameObject::Create(*gce::GameManager::GetActiveScene());
	std::string* name = new std::string(std::string(m_pOwner->GetName()) + "_AttackRadiusChecker");
	m_radiusChecker->SetName(name->c_str());
	m_radiusChecker->SetParent(*m_pOwner);
	m_radiusChecker->transform.SetLocalPosition({ 0.0f, 0.0f, 0.0f });
	m_radiusChecker->transform.SetWorldScale({ sightRadius, sightRadius, sightRadius });

	if (debugMode) {
		gce::MeshRenderer* meshRenderer = m_radiusChecker->AddComponent<gce::MeshRenderer>();
		meshRenderer->SetGeometry(gce::SHAPES.SPHERE);
	}

	m_checkerCollider = m_radiusChecker->AddComponent<gce::SphereCollider>();
	m_checkerCollider->localScale = 1.0f;
	m_checkerCollider->isTrigger = true;
	m_checkerCollider->localRay = 0.5f;

	m_checkerCollider->collisionEnterEvent.AddListener(&AIController::OnTriggerEnter, this);
	m_checkerCollider->collisionExitEvent.AddListener(&AIController::OnTriggerExit, this);

	m_loseSightTimer->OnCompleted.AddListener(&AIController::LoseSight, this);
	m_loseSightTimer->Reset();

	m_healthStat->OnDeath.AddListener(&AIController::OnDeath, this);
}

void AIController::Destroy()
{
	//delete m_loseSightTimer;
}

void AIController::Update() {
	// Death animation takes priority over damage animation
	if (m_play_death_anim) {
		m_death_anim_timer += gce::GameManager::DeltaTime();
		float progress = m_death_anim_timer / m_death_anim_duration;

		if (progress >= 1.0f) {
			// Animation complete, destroy the AI
			m_pOwner->Destroy();
			return;
		}

		// Spinning effect - rotate around Y axis
		float spinSpeed = 720.0f; // 2 full rotations per second
		float currentAngle = m_death_anim_timer * spinSpeed;
		gce::Quaternion spinRotation = gce::Quaternion::RotationAxis({ 0.0f, 1.0f, 0.0f }, currentAngle * (gce::PI / 180.0f));

		// Stretch and squash effect using sine wave
		float squashFrequency = 8.0f;
		float squashAmount = std::sin(m_death_anim_timer * squashFrequency * gce::PI) * 0.3f * (1.0f - progress);
		
		// Scale down over time with stretch/squash
		float baseScale = 1.0f - progress; // Linearly scale from 1 to 0
		gce::Vector3f32 scale = {
			baseScale * (1.0f + squashAmount), // X - squash horizontally
			baseScale * (1.0f - squashAmount), // Y - stretch vertically (inverse of X)
			baseScale * (1.0f + squashAmount)  // Z - squash depth
		};

		m_pOwner->transform.SetLocalRotation(spinRotation);
		m_pOwner->transform.SetLocalScale(scale);

		return; // Skip all other behavior during death animation
	}

	if (m_play_damage_anim) {
		gce::Vector3f32 knockbackDir = m_pOwner->transform.GetWorldPosition() - m_player->transform.GetWorldPosition();
		knockbackDir.y = 0.0f;
		knockbackDir.SelfNormalize();

		static float timer = 0.0f;
		timer += gce::GameManager::DeltaTime();

		// Calculate a X rotation axis perpendicular to the knockback direction
		gce::Vector3f32 rotationAxis = knockbackDir.CrossProduct({ 0.0f, 1.0f, 0.0f });

		// Appply a spring like rotation animation to simulate a hit reaction
		float angle = std::sin(timer * 20.0f) * (1.0f - (timer / 1.0f)) * 30.0f; // 1 second duration
		gce::Quaternion targetRotation = gce::Quaternion::RotationAxis(rotationAxis, angle * (gce::PI / 180.0f));
		m_pOwner->transform.SetLocalRotation(targetRotation);

		// Also apply a spring like knockback movement
		float knockbackDistance = std::sin(timer * 20.0f) * (1.0f - (timer / 1.0f)) * 0.5f; // 1 second duration
		m_pOwner->transform.WorldTranslate(knockbackDir * knockbackDistance);
		if (timer >= 1.0f) {
			timer = 0.0f;
			m_play_damage_anim = false;
			// Reset rotation
			m_pOwner->transform.SetLocalRotation(gce::Quaternion());
		}
	}

	switch (m_currentState) {
	case AIState::Idle:
		OnIdle(m_player);
		break;
	case AIState::Attack:
		OnAttack(m_player);
		break;
	default:
		break;
	}
	m_radiusChecker->transform.SetWorldScale({ sightRadius, sightRadius, sightRadius });
}

void AIController::OnTriggerEnter(gce::GameObject* _other) {
	if (_other->GetName() == "Player") {
		m_currentState = AIState::Attack;
		m_loseSightTimer->Stop();
	}
}
void AIController::OnTriggerExit(gce::GameObject* _other) {
	if (_other->GetName() == "Player") {
		if (m_currentState == AIState::Attack)
			m_loseSightTimer->Restart();
	}
}

void AIController::LoseSight() {
	m_currentState = AIState::Idle;
}

void AIController::OnTakeDammage(gce::GameObject* _gm) {
	m_play_damage_anim = true;
}

void AIController::OnDeath(gce::GameObject* _gm) {
	m_play_death_anim = true;
	m_death_anim_timer = 0.0f;
	m_currentState = AIState::Idle; // Stop attacking
}