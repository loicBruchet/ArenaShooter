#pragma once
#include "Framework/Prefab.h"

class ElevatorPrefab : public Prefab {
public:
	ElevatorPrefab(gce::Scene& scene,  json* _properties = nullptr);
	virtual ~ElevatorPrefab();
protected:
private:
};
REGISTER_PREFAB(ElevatorPrefab)

