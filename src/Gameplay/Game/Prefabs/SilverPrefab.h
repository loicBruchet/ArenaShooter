#pragma once
#include "Framework/Prefab.h"

class SilverPrefab : public Prefab {
public:
	SilverPrefab(gce::Scene& _scene, json* _properties = nullptr);
	virtual ~SilverPrefab();
};	
REGISTER_PREFAB(SilverPrefab)
