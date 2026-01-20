#pragma once

#include "Framework/Prefab.h"

class ExplosiveProjectilePrefab : public Prefab {
public:
	ExplosiveProjectilePrefab(gce::Scene& _scene, json* _properties = nullptr);
	virtual ~ExplosiveProjectilePrefab();
protected:
private:
};
REGISTER_PREFAB(ExplosiveProjectilePrefab)