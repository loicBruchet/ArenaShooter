#pragma once

#include "Framework/Prefab.h"

class MachineGunProjectilePrefab : public Prefab	
{
public:
	MachineGunProjectilePrefab(gce::Scene& _scene, json* _properties = nullptr);
	virtual ~MachineGunProjectilePrefab();
};
REGISTER_PREFAB(MachineGunProjectilePrefab)
