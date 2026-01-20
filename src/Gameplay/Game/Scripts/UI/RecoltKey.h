#pragma once

#include <Engine.h>
#include "Game/Utils/Timer.h"

DECLARE_SCRIPT(RecoltKey, ScriptFlag::Start | ScriptFlag::Update)
public:
	virtual void Start() override;
	virtual void Update() override;

private:
	gce::Scene* scene = nullptr;
	gce::SphereCollider* m_collider = nullptr;

	void OnCollisionEnter(gce::GameObject* _other);

	END_SCRIPT