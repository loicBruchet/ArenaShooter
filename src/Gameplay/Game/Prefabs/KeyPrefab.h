#pragma once
#include "Framework/Prefab.h"
#include "../Tags.h" 

class KeyPrefab : public Prefab {
public:
	KeyPrefab(gce::Scene& _scene, json* _properties = nullptr);
	virtual ~KeyPrefab();
protected:
private:
};
REGISTER_PREFAB(KeyPrefab)