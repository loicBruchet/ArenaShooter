#pragma once

#include "Framework/Prefab.h"

class DixoPrefab : public Prefab {
public:
	DixoPrefab(gce::Scene& _scene, json* _properties = nullptr);
	virtual ~DixoPrefab();
protected:
private:
};
REGISTER_PREFAB(DixoPrefab)