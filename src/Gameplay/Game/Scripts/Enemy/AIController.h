#pragma once

#include <Engine.h>
#include "Game/Utils/Timer.h"
#include "Game/Scripts/Stats/HealthStat.h"

enum class AIState {
	Idle,
	Attack
};

DECLARE_SCRIPT(AIController, ScriptFlag::Update | ScriptFlag::Start | ScriptFlag::Destroy)
public:
	float sightRadius = 10.0f; // Radius in which the AI can see the player & start attacking
	float loseSightTime = 2.0f; // Time in seconds before the AI stops attacking after losing sight of the player
	bool debugMode = false; // If true, will render the attack radius

	virtual void OnIdle(gce::GameObject* _player) = 0;
	virtual void OnAttack(gce::GameObject* _player) = 0;
	virtual void Destroy() override;

	virtual void Start() override;
	virtual void Update() override;

protected:
	HealthStat* m_healthStat = nullptr;
	Timer* m_loseSightTimer = new Timer(loseSightTime);

private:
	void OnTriggerEnter(gce::GameObject* _other);
	void OnTriggerExit(gce::GameObject* _other);

	void LoseSight();

	void OnTakeDammage(gce::GameObject* _gm);
	void OnDeath(gce::GameObject* _gm);

private:
	bool m_play_damage_anim = false;
	bool m_play_death_anim = false;
	float m_death_anim_timer = 0.0f;
	const float m_death_anim_duration = 1.5f; // Duration of death animation in seconds





	AIState m_currentState = AIState::Idle;
	gce::GameObject* m_radiusChecker = nullptr;
	gce::SphereCollider* m_checkerCollider = nullptr;

	gce::GameObject* m_player = nullptr;
END_SCRIPT