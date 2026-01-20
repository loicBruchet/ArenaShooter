#include "UIGestion.h"

#include "Framework/PrefabRegistry.h"
#include "Framework/Prefab.h"
void UIGestionHealth::CallUpdate(gce::GameObject* _gm)
{
	for (int i = 0; i < m_healthStat->currentHealth; i++)
	{
		gce::GameObject& obj = UI[i]->GetRootObject();
		gce::Vector3f32 pos = obj.transform.GetLocalPosition();
		gce::Vector3f32 scale = obj.transform.GetLocalScale();
		gce::UiImage* ui = obj.GetComponent<gce::UiImage>();

		if (!ui) continue;

		ui->btmBrush = fullBrushes[i];
		obj.transform.SetLocalPosition(pos);
		obj.transform.SetLocalScale(scale);
	}
	
	for (int j = m_healthStat->currentHealth; j < m_healthStat->maxHealth; j++)
	{
		gce::GameObject& obj = UI[j]->GetRootObject();
		gce::Vector3f32 pos = obj.transform.GetLocalPosition();
		gce::Vector3f32 scale = obj.transform.GetLocalScale();
		gce::UiImage* ui = obj.GetComponent<gce::UiImage>();

		if (!ui) continue;

		ui->btmBrush = emptyBrushes[j];
		obj.transform.SetLocalPosition({ pos.x, pos.y, pos.z });
		obj.transform.SetLocalScale(scale);
	}

	isFading = true;
	fadeTimer = 0.0f;
}
void UIGestionHealth::Start()
{
    m_healthStat = m_scene->GetObjectByName("Player")->GetScript<HealthStat>();
	m_healthStat->OnHealthChanged.AddListener(&UIGestionHealth::CallUpdate, this);
}

void UIGestionHealth::Update() {
	if (isFading)
	{
		fadeTimer += gce::GameManager::DeltaTime();
		
		float progress = fadeTimer / fadeDuration;
		
		if (progress < 0.5f)
		{
			float opacity = progress * 2.0f;
			dammageUIImage->SetOpacity(opacity);
		}
		else if (progress < 1.0f)
		{
			float opacity = (1.0f - progress) * 2.0f;
			dammageUIImage->SetOpacity(opacity);
		}
		else
		{
			isFading = false;
			fadeTimer = 0.0f;
		}
	}
	else
	{
		float healthPercentage = static_cast<float>(m_healthStat->currentHealth) / static_cast<float>(m_healthStat->maxHealth);
		float baseOpacity = (1.0f - healthPercentage) * 0.7f + 0.15f;
		
		float blinkSpeed = 3.0f + (1.0f - healthPercentage) * 6.0f;
		
		static float blinkTimer = 0.0f;
		blinkTimer += gce::GameManager::DeltaTime() * blinkSpeed;
		
		float blinkValue = (std::sin(blinkTimer) + 1.0f) * 0.5f;
		
		float finalOpacity = baseOpacity * blinkValue;
		dammageUIImage->SetOpacity(finalOpacity);
	}
}

void UIGestionHealth::LoadFullBrush(const std::string& path)
{
	fullBrush = new gce::BitMapBrush(path);
}
void UIGestionHealth::LoadEmptyBrush(const std::string& path)
{
	emptyBrush = new gce::BitMapBrush(path);
}
void UIGestionHealth::InitializeUI()
{
	m_scene = gce::GameManager::GetActiveScene();
	for (int i = 0; i < 10; i++)
	{
		Prefab* healthUi = PREFAB_REGISTRY.CreatePrefab(name, *m_scene);



		gce::Vector3f32 pos = m_pOwner->transform.GetLocalPosition();
		healthUi->GetRootObject().transform.SetLocalPosition({ pos.x + i * 30.0f,pos.y ,pos.z });
		healthUi->GetRootObject().transform.SetLocalScale({ 60.0f,60.0f,0.0f });
		UI.push_back(healthUi);

		fullBrushes.push_back(new gce::BitMapBrush("res/ArenaShooter/UI/2PV.png"));
		emptyBrushes.push_back(new gce::BitMapBrush("res/ArenaShooter/UI/0PV.png"));
	}

	// Create a fullscreen dammage UI
	dammageUI = &gce::GameObject::Create(*m_scene);
	dammageUI->SetName("DammageUI");
	dammageUI->transform.SetLocalPosition({ 1920.0f / 2.0f,1080.0f/2.0f, 0.0f });
	dammageUI->transform.SetLocalScale({ 1920.0f,1080.0f, 0.0f });
	dammageUIImage = dammageUI->AddComponent<gce::UiImage>();
	dammageUIImage->rectImage = new gce::RectanglePosF(0.0f, 0.0f, 1920.0f, 1080.0f);
	dammageUIImage->btmBrush = new gce::BitMapBrush("res/ArenaShooter/UI/DammageUI.png");
	dammageUIImage->InitializeImage({ 0.0f, 0.0f }, { 1920.0f, 1080.0f }, 0.0f);
}