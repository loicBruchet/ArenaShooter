#pragma once
#include <Engine.h>

DECLARE_SCRIPT(Elevator,  ScriptFlag::Start)

public:


	virtual void Start() override;


	void OnCollisionEnter(gce::GameObject* _other);

	void OnCollisionEnterUp(gce::GameObject* other);

	void OnCollisionExitUp(gce::GameObject* other);



private:
	gce::BoxCollider* m_boxCollider = nullptr;

	bool m_hasNotCollide = true;

	float32 m_speed = 10.0f;
END_SCRIPT

