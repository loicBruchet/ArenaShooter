#pragma once

#include "Framework/Prefab.h"

class NexronPrefab : public Prefab {
public:
	NexronPrefab(gce::Scene& _scene, json* _properties = nullptr);
	virtual ~NexronPrefab();
protected:
private:
};
REGISTER_PREFAB(NexronPrefab)