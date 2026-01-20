#pragma once

#include <Engine.h>
#include "Game/Scripts/Enemy/AIController.h"

enum class NexronState {
	Idle,
	SlamAttack,
	DashAttack
};

DECLARE_CHILD_SCRIPT(NexronAI, AIController, ScriptFlag::Start)
public:
	virtual void OnIdle(gce::GameObject* _player) override;
	virtual void OnAttack(gce::GameObject* _player) override;
	virtual void Start() override;

	void OnDammageSphereEnter(gce::GameObject* _other);

private:
	// Attacks
	void SlamAttack(gce::GameObject* _player);
	void DashAttack(gce::GameObject* _player);
	void ComeBackToPosition();

	// Fight logic helpers
	void StartNewWaveGroup();
	void StartNewAttackSequence();
	NexronState GetRandomAttackState();
	int RandomRange(int min, int max);

	void OnDeath(gce::GameObject* _gm);

private:
	// Damage sphere
	gce::GameObject* m_dammageSphere = nullptr;
	gce::SphereCollider* m_dammageSphereCollider = nullptr;

	// State
	NexronState m_currentState = NexronState::Idle;

	// Positions
	gce::Vector3f32 m_initialPosition;
	gce::Vector3f32 m_dashStartPosition;
	gce::Vector3f32 m_dashTargetPosition;

	// Dash data
	bool m_dashInitialized = false;
	float m_maxDashDistance = 30.0f;
	float m_dashProgress = 0.0f;
	float m_dashDuration = 0.8f;
	float m_dashStopDistance = 3.0f;

	// Fighting (kinda lol)
	bool m_fightStarted = false;
	bool m_waitingFightStart = false;

	float m_fightStartTimer = 0.0f;

	int m_attacksRemainingInSequence = 0;
	int m_wavesRemaining = 0;

	float m_idleBetweenWavesTimer = 0.0f;
	float m_idleBetweenWavesDuration = 2.0f;

	bool m_pendingNewAttack = false;
END_SCRIPT