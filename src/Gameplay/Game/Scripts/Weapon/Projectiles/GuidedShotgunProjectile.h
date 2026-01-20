#pragma once
#include <Engine.h>

#include "Game/Scripts/Weapon/Projectile.h"
#include "Game/Utils/Timer.h"
DECLARE_CHILD_SCRIPT(GuidedShotgunProjectile, Projectile, ScriptFlag::Start | ScriptFlag::Update | ScriptFlag::Destroy)
public:
	gce::GameObject* target = nullptr;

	
	virtual void Start() override;
	virtual void Update() override;
	virtual void Destroy() override;

	void OnCollisionEnterHoming(gce::GameObject* other);

	virtual void OnHit(gce::GameObject* _other) override;

private:
	bool m_hasTarget = false;
	float m_homingRadius = 10.0f;	
END_SCRIPT