#pragma once

#include "Framework/Prefab.h"

class KeyUIPrefab : public Prefab {
public:
	KeyUIPrefab(gce::Scene& _scene, json* _properties = nullptr);
	virtual ~KeyUIPrefab();
protected:
private:
};
REGISTER_PREFAB(KeyUIPrefab)