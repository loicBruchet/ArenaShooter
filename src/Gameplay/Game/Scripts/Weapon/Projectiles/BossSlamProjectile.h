#pragma once

#include "../Projectile.h"

DECLARE_CHILD_SCRIPT(BossSlamProjectile, Projectile, ScriptFlag::Start | ScriptFlag::Update)
public:
	virtual void Start() override;
	virtual void Update() override;

protected:
	virtual void OnHit(gce::GameObject* _other) override;

private:
	float m_time = 0.0f;
	bool m_exploding = false;
END_SCRIPT