#pragma once

#include <Engine.h>
#include "Game/Utils/Timer.h"

DECLARE_SCRIPT(Projectile, ScriptFlag::Start | ScriptFlag::Update)
public:
	float speed = 50.0f;
	float lifetime = 2.0f;

	bool destroyOnHit = true;
	bool destroyOnLifetimeEnd = true;

	virtual void Start() override;
	virtual void Update() override;

protected:
	virtual void OnHit(gce::GameObject* _other) = 0;

	gce::SphereCollider* m_collider;

	Timer* m_timer = new Timer(lifetime);

private:
	


	void OnCollisionEnter(gce::GameObject* _other);

END_SCRIPT