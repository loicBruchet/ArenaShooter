#pragma once

#include <Engine.h>
#include <random>

DECLARE_SCRIPT(PlayerCamera, ScriptFlag::Start | ScriptFlag::Update)
public:
	float32 lookSpeed = 0.002f; // Speed of camera rotation (no units because idk XD)

	virtual void Start() override;
	virtual void Update() override;

	void CameraShake(float32 intensity, float32 duration);

	// Call this from anywhere to trigger the camera shake on the active player camera
	static void Shake(float32 intensity, float32 duration);

private:
	// Camera shake state
	bool m_isShaking = false;
	float32 m_shakeIntensity = 0.0f;
	float32 m_shakeDuration = 0.0f;
	float32 m_shakeElapsed = 0.0f;
	gce::Vector3f32 m_originalLocalPositionStart = { 0.0f, 0.0f, 0.0f };
	gce::Vector3f32 m_originalLocalPosition = { 0.0f, 0.0f, 0.0f };
	// RNG for shake offsets
	std::mt19937* m_rng = nullptr; // J'ai aucune foutre idée de ce que c'est, j'ai juste grab sur stack-overflow
	std::uniform_real_distribution<float>* m_dist = nullptr; // J'ai aucune foutre idée de ce que c'est, j'ai juste grab sur stack-overflow

	static PlayerCamera* s_instance;

END_SCRIPT