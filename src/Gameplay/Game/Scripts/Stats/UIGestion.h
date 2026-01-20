#pragma once

#include <Engine.h>

#include "HealthStat.h"

class Prefab;


DECLARE_SCRIPT(UIGestionHealth,  ScriptFlag::Start | ScriptFlag::Update)
public:
	gce::BitMapBrush* fullBrush = nullptr;
	gce::BitMapBrush* emptyBrush = nullptr;

	std::vector<Prefab*> UI;

	std::vector<gce::BitMapBrush*> fullBrushes;

	std::vector<gce::BitMapBrush*> emptyBrushes;

	gce::GameObject* dammageUI = nullptr;
	gce::UiImage* dammageUIImage = nullptr;

	float fadeDuration = 0.25f;

	void CallUpdate(gce::GameObject* _gm) ;
	virtual	void Start() override;
	virtual	void Update() override;

	void LoadFullBrush(const std::string& path);
	void LoadEmptyBrush(const std::string& path);

	void InitializeUI();

	
	std::string name = "HealthPrefab";

private:

	HealthStat* m_healthStat = nullptr;
	gce::Scene* m_scene = nullptr;

	bool isFading = false;
	float fadeTimer = 0.0f;


END_SCRIPT
