#pragma once

#include "Framework/Prefab.h"

class MenuStartPrefab : public Prefab
{
public:
	MenuStartPrefab(gce::Scene& _scene, json* _properties = nullptr);
	virtual ~MenuStartPrefab();
private:
	void OnClickQuit();
	void OnClickStart();
	
};


REGISTER_PREFAB(MenuStartPrefab)
