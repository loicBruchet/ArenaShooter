#pragma once

#include "Framework/Prefab.h"
#include "../Tags.h"
class ChromePrefab : public Prefab {
public:
	ChromePrefab(gce::Scene& _scene, json* _properties = nullptr);
	virtual ~ChromePrefab();
protected:
private:
};
REGISTER_PREFAB(ChromePrefab)