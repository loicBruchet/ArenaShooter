#pragma once

#include "Framework/Prefab.h"

class ShieldPrefab : public Prefab
{
public:
	ShieldPrefab(gce::Scene& _scene, json* _properties = nullptr);
	virtual ~ShieldPrefab();
private:
}; 
REGISTER_PREFAB(ShieldPrefab)