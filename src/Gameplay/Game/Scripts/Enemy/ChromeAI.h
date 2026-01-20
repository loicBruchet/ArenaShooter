#pragma once

#include <Engine.h>

#include "Game/Scripts/Enemy/AIController.h"
#include "Game/Scripts/Weapon/Weapon.h"

DECLARE_CHILD_SCRIPT(ChromeAI, AIController, ScriptFlag::Start)
public:
	virtual void OnIdle(gce::GameObject* _player) override;
	virtual void OnAttack(gce::GameObject* _player) override;

	virtual void Start() override;

	void OnShoot();

private:
	bool TurnTowardsTarget(gce::GameObject* _target);

private:
	Weapon* m_weapon = nullptr;
END_SCRIPT