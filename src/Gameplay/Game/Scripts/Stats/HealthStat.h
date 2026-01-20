#pragma once

#include <Engine.h>

DECLARE_SCRIPT(HealthStat, ScriptFlag::Update | ScriptFlag::Start | ScriptFlag::Destroy)
public:
	int maxHealth = 100;
	int currentHealth = 100;

	gce::Event<gce::GameObject*> OnDeath; // An Event that triggers once when health reaches 0

	gce::Event<gce::GameObject*> OnHealthChanged; // An Event that triggers every time the health changes

	gce::Event<gce::GameObject*> OnTakeDammage; // An Event that triggers every time the object takes dammage
	gce::Event<gce::GameObject*> OnHeal; // An Event that triggers every time the object is healed

	void Init(int _maxHealth = 100);
	void Die(bool _destroy = false);

	void Heal(int _ammount);
	void TakeDammage(int _ammount);

	virtual void Start() override;
	virtual void Update() override;
	virtual void Destroy() override;

	const bool IsAlive() const;

private:
	bool alive = true;
	gce::SphereCollider* collider = nullptr;

END_SCRIPT