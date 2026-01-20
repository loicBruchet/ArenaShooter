#pragma once

#include <Engine.h>

#include "Game/Scripts/Enemy/AIController.h"
#include "Framework/Scripts/Agent.h"
#include "Game/Utils/Timer.h"

DECLARE_CHILD_SCRIPT(SilverAI, AIController, ScriptFlag::Start)
public:
	float attackDelay = 1.0f; // The delay after Silver turns towards the player before chasing it
	float attackCooldown = 2.0f; // The cooldown between each attack

	virtual void OnIdle(gce::GameObject* _player) override;
	virtual void OnAttack(gce::GameObject* _player) override;
	virtual void Destroy() override;

	virtual void Start() override;

	void OnCollisionWithPlayer(gce::GameObject* _player);

private:
	bool m_waiting = false;
	bool TurnTowardsTarget(gce::GameObject* _target);
	Timer* m_delayTimer = new Timer(attackDelay);
	//Timer* m_attackCooldownTimer = new Timer(attackCooldown);

	gce::SphereCollider* m_silverCollider = nullptr;

	Agent* m_agent = nullptr;
END_SCRIPT