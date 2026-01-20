#pragma once

#include "Framework/Prefab.h"

class BossSlamProjectilePrefab : public Prefab {
public:
	BossSlamProjectilePrefab(gce::Scene& _scene, json* _properties = nullptr);
	virtual ~BossSlamProjectilePrefab();
protected:
private:
};
REGISTER_PREFAB(BossSlamProjectilePrefab)