#pragma once

#include "Framework/Prefab.h"


class PlayerPrefab : public Prefab {
public:
	PlayerPrefab(gce::Scene& _scene, json* _properties = nullptr);
	virtual ~PlayerPrefab();
protected:
private:
};
REGISTER_PREFAB(PlayerPrefab)