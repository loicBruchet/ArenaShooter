#pragma once

#include <Engine.h>
#include "Game/Utils/Timer.h"

DECLARE_SCRIPT(Healler, ScriptFlag::Start | ScriptFlag::Update)
public:
	int healAmount = 10000000;
	virtual void Start() override;
	virtual void Update() override;

private:

	gce::SphereCollider* m_collider;

	void OnCollisionEnter(gce::GameObject* _other);

	END_SCRIPT