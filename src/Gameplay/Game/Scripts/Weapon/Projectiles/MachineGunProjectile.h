#pragma once

#include "../Projectile.h"

DECLARE_CHILD_SCRIPT(MachineGunProjectile, Projectile, ScriptFlag::Start)
public:
	virtual void Start() override;

protected:
	virtual void OnHit(gce::GameObject* _other) override;

private:

END_SCRIPT