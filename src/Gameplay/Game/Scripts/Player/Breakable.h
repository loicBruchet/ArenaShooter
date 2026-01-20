#pragma once

#include <Engine.h>

#include "Game/Scripts/Stats/HealthStat.h"

DECLARE_SCRIPT(Breakable, ScriptFlag::Start)
public:
	virtual void Start() override;

	void OnTakeDammage(gce::GameObject* _gm);

private:
	HealthStat* m_healthStat = nullptr;
END_SCRIPT