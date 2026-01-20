#pragma once

#include "Framework/Prefab.h"

class ShotgunProjectilePrefab : public Prefab {
public:
	ShotgunProjectilePrefab(gce::Scene& _scene, json* _properties = nullptr);
	virtual ~ShotgunProjectilePrefab();
protected:
private:
};
REGISTER_PREFAB(ShotgunProjectilePrefab)