#pragma once

#include "Framework/Prefab.h"

class HUDPrefab : public Prefab {
public:
	HUDPrefab(gce::Scene& _scene, json* _properties = nullptr);
	virtual ~HUDPrefab();
protected:
private:
};
REGISTER_PREFAB(HUDPrefab)