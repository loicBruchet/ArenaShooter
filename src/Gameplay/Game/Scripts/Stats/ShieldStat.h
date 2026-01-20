#pragma once

#include <Engine.h>

#include "HealthStat.h"

#include "Game/Utils/Timer.h"
DECLARE_SCRIPT(ShieldStat, ScriptFlag::Start | ScriptFlag::Update | ScriptFlag::Destroy)
public:
	int maxShield = 100;
	int currentShield = 100;

	float regenerationRate = 1.0f; // Shield points per second
	float renerationDelay = 3.0f; // The time needed after taking damage before regeneration starts

	gce::Event<gce::GameObject*> OnBreak; // An Event that triggers once when shield reaches 0

	gce::Event<gce::GameObject*> OnShieldChanged; // An Event that triggers every time the shield changes

	gce::Event<gce::GameObject*> OnTakeDammage; // An Event that triggers every time the shield takes dammage
	gce::Event<gce::GameObject*> OnRegenerate; // An Event that triggers every time the shield regenerates


	void Init(int _maxShield = 100);
	void Break();

	void Regenerate(int _ammount);
	void TakeDammage(int _ammount);

	virtual void Start() override;
	virtual void Update() override;
	virtual void Destroy() override;

	const bool IsBroken() const;

private:
	bool canRegenerate = true;

	void RegenerateOnRateTimer();
	void StartRegeneration();

private:
	bool broken = false;

	Timer rateTimer = Timer(1 / regenerationRate);
	Timer delayTimer = Timer(renerationDelay);

	HealthStat* health = nullptr;

END_SCRIPT