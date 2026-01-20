#include "PlayerCamera.h"
#include <random>
#include <ctime>

PlayerCamera* PlayerCamera::s_instance = nullptr;

void PlayerCamera::Start() {
	gce::GameManager::LockCursor();

	// record original local position
	m_originalLocalPosition = m_pOwner->transform.GetLocalPosition();
	m_originalLocalPositionStart = m_pOwner->transform.GetLocalPosition();

	// init RNG and distribution
	m_rng = new std::mt19937((unsigned)std::time(nullptr) ^ (uintptr_t)this);
	m_dist = new std::uniform_real_distribution<float>(-1.0f, 1.0f);

	// register static instance so Shake() can be called globally
	s_instance = this;
}

void PlayerCamera::Update() {
	gce::Vector2f32 deltaMousePos = gce::GameManager::GetMouseDelta();

	// rotate on the Y axis (left/right)
	//m_pOwner->transform.LocalRotate({ 0.0f, deltaMousePos.x * lookSpeed, 0.0f });

	// rotate camera on the X axis (up/down)
	//gce::GameObject* cameraObject = m_pOwner->GetChildren()[0];
	//static float32 pitch = cameraObject->transform.GetLocalRotation().GetX();
	//pitch += deltaMousePos.y * lookSpeed;
	//pitch = gce::Clamp(pitch, -gce::PI / 2.0f + 0.1f, gce::PI / 2.0f - 0.1f);
	//cameraObject->transform.SetLocalRotation({ pitch, 0.0f, 0.0f });

	static float32 pitch = m_pOwner->transform.GetLocalRotation().GetX();
	pitch += deltaMousePos.y * lookSpeed;
	pitch = gce::Clamp(pitch, -gce::PI / 2.0f + 0.1f, gce::PI / 2.0f - 0.1f);
	m_pOwner->transform.SetLocalRotation({ pitch, 0.0f, 0.0f });

	gce::GameObject* playerObject = m_pOwner->GetParent();
	playerObject->transform.LocalRotate({ 0.0f, deltaMousePos.x * lookSpeed, 0.0f });

	// Camera shake update
	if (m_isShaking) {
		float32 dt = gce::GameManager::DeltaTime();
		m_shakeElapsed += dt;
		if (m_shakeElapsed >= m_shakeDuration) {
			// end shake
			m_isShaking = false;
			m_shakeElapsed = 0.0f;
			m_pOwner->transform.SetLocalPosition(m_originalLocalPosition);
		}
		else {
			// decay over time
			float32 progress = m_shakeElapsed / m_shakeDuration;
			float32 decay = 1.0f - progress;

			float x = (*m_dist)(*m_rng);
			float y = (*m_dist)(*m_rng);
			float z = (*m_dist)(*m_rng);

			gce::Vector3f32 offset = { x, y, z };
			offset *= (m_shakeIntensity * decay);

			m_pOwner->transform.SetLocalPosition(m_originalLocalPosition + offset);
		}
	}
	else {
		// ensure smooth lerp to original position
		gce::Vector3f32 currentPos = m_pOwner->transform.GetLocalPosition();
		gce::Vector3f32 newPos = gce::Vector3f32::Lerp(currentPos, m_originalLocalPositionStart, gce::GameManager::DeltaTime() * 5.0f);
		m_pOwner->transform.SetLocalPosition(newPos);
	}
}

void PlayerCamera::CameraShake(float32 intensity, float32 duration) {
	if (intensity <= 0.0f || duration <= 0.0f) return;

	m_shakeIntensity = intensity;
	m_shakeDuration = duration;
	m_shakeElapsed = 0.0f;
	//m_originalLocalPosition = m_pOwner->transform.GetLocalPosition();
	m_isShaking = true;
}

void PlayerCamera::Shake(float32 intensity, float32 duration) {
	if (s_instance == nullptr) return;
	s_instance->CameraShake(intensity, duration);
}