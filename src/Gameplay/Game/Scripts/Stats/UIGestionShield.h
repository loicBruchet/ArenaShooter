#pragma once

#include <Engine.h>
#include "ShieldStat.h"

class Prefab;


DECLARE_SCRIPT(UIGestionShield, ScriptFlag::Start|  ScriptFlag::Update)
public:
	gce::BitMapBrush* fullBrush = nullptr;
	gce::BitMapBrush* emptyBrush = nullptr;

	std::vector<Prefab*> UI;
	std::vector<gce::BitMapBrush*> fullBrushes;
	std::vector<gce::BitMapBrush*> emptyBrushes;

	gce::GameObject* dammageUI = nullptr;
	gce::UiImage* dammageUIImage = nullptr;

	float fadeDuration = 0.5f;

	void CallUpdate(gce::GameObject* _gm);
	virtual	void Start() override;
	virtual void Update() override;

	void LoadFullBrush(const std::string& path);
	void LoadEmptyBrush(const std::string& path);

	void InitializeUI();

	std::string name = "ShieldPrefab";

private:

	ShieldStat* m_shieldStat = nullptr;
	gce::Scene* m_scene = nullptr;

	bool isFading = false;
	float fadeTimer = 0.0f;


END_SCRIPT