#pragma once
#include "Framework/Prefab.h"
#include "../Tags.h"
class CollectablePrefab : public Prefab {
public:
	CollectablePrefab(gce::Scene& _scene, json* _properties = nullptr);
	virtual ~CollectablePrefab();
protected:
private:
};
REGISTER_PREFAB(CollectablePrefab)
