#pragma once

#include "Framework/Prefab.h"

class HealthPrefab : public Prefab {
public:
	HealthPrefab(gce::Scene& _scene, json* _properties = nullptr);
	virtual ~HealthPrefab();
private:
};
REGISTER_PREFAB(HealthPrefab)