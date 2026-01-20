#pragma once
#include "Framework/Prefab.h"
#include "../Tags.h" 

class BreakableObjectPrefab : public Prefab
{
public:
	BreakableObjectPrefab(gce::Scene& _scene, json* _properties = nullptr);
	virtual ~BreakableObjectPrefab();
private:
};
REGISTER_PREFAB(BreakableObjectPrefab)

