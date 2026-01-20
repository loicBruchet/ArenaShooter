#pragma once
#include "Framework/Prefab.h"
class GuidedShotgunProjectilePrefab : public Prefab {
public:
	GuidedShotgunProjectilePrefab(gce::Scene& _scene, json* _properties = nullptr);
	virtual ~GuidedShotgunProjectilePrefab();
};
REGISTER_PREFAB(GuidedShotgunProjectilePrefab)