#pragma once

#include "../Projectile.h"

DECLARE_CHILD_SCRIPT(ExplosiveProjectile, Projectile, ScriptFlag::Start | ScriptFlag::Update)
public:
	virtual void Start() override;
	virtual void Update() override;

protected:
	virtual void OnHit(gce::GameObject* _other) override;
	
	void OnStartExplosion();

private:
	bool m_hit = false;
	Timer* m_delay = new Timer(1);
	float m_time = 0.0f;
	gce::GameObject* m_target = nullptr;

	bool m_exploding = false;
	bool m_exploded = false;
END_SCRIPT