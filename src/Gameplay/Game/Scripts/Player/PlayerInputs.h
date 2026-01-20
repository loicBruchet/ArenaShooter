#pragma once

#include <Engine.h>

#include "PlayerMovements.h"
#include "../Weapon/Weapon.h"

DECLARE_SCRIPT(PlayerInputs, ScriptFlag::Start | ScriptFlag::Update)
public:
	virtual void Start() override;
	virtual void Update() override;

private:
	gce::TextRenderer* m_ammoText = nullptr;
	PlayerMovements* m_movements = nullptr;
	Weapon* m_weapon = nullptr;

END_SCRIPT