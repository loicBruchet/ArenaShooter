#include "UIGestionShield.h"

#include "Framework/PrefabRegistry.h"
#include "Framework/Prefab.h"

void UIGestionShield::CallUpdate(gce::GameObject* _gm)
{
	for (int i = 0; i < m_shieldStat->currentShield; i++)
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
	
	for (int j = m_shieldStat->currentShield; j < m_shieldStat->maxShield; j++)
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

void UIGestionShield::Start()
{

	m_shieldStat = m_scene->GetObjectByName("Player")->GetScript<ShieldStat>();
	m_shieldStat->OnShieldChanged.AddListener(&UIGestionShield::CallUpdate, this);
}
void UIGestionShield::Update()
{
	if (gce::GetKeyDown(gce::Keyboard::W))
		m_shieldStat->TakeDammage(1);

	if (m_shieldStat->IsBroken()) {
		dammageUIImage->SetOpacity(0.0f);
		isFading = false;
		fadeTimer = 0.0f;
		return;
	}

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
		float shieldPercentage = static_cast<float>(m_shieldStat->currentShield) / static_cast<float>(m_shieldStat->maxShield);
		float baseOpacity = (1.0f - shieldPercentage) * 0.7f + 0.15f;
		
		float blinkSpeed = 3.0f + (1.0f - shieldPercentage) * 6.0f;
		
		static float blinkTimer = 0.0f;
		blinkTimer += gce::GameManager::DeltaTime() * blinkSpeed;
		
		float blinkValue = (std::sin(blinkTimer) + 1.0f) * 0.5f;
		
		float finalOpacity = baseOpacity * blinkValue;
		dammageUIImage->SetOpacity(finalOpacity);
	}
}
void UIGestionShield::LoadFullBrush(const std::string& path)
{
	fullBrush = new gce::BitMapBrush(path);
}
void UIGestionShield::LoadEmptyBrush(const std::string& path)
{
	emptyBrush = new gce::BitMapBrush(path);
}
void UIGestionShield::InitializeUI()
{
	m_scene = gce::GameManager::GetActiveScene();
	for (int i = 0; i < 5; i++)
	{
		Prefab* shieldUi = PREFAB_REGISTRY.CreatePrefab(name, *m_scene);


		gce::Vector3f32 pos = m_pOwner->transform.GetLocalPosition();
		shieldUi->GetRootObject().transform.SetLocalPosition({ pos.x + i * 35.0f,pos.y ,pos.z });
		shieldUi->GetRootObject().transform.SetLocalScale({ 50.0f,50.0f,0.0f });
		UI.push_back(shieldUi);

		fullBrushes.push_back(new gce::BitMapBrush("res/ArenaShooter/UI/Shield_dark.png"));
		emptyBrushes.push_back(new gce::BitMapBrush("res/ArenaShooter/UI/Shield_light.png"));
	}

	dammageUI = &gce::GameObject::Create(*m_scene);
	dammageUI->SetName("ShieldDammageUI");
	dammageUI->transform.SetLocalPosition({ 1920.0f / 2.0f,1080.0f/2.0f, 0.0f });
	dammageUI->transform.SetLocalScale({ 1920.0f,1080.0f, 0.0f });
	dammageUIImage = dammageUI->AddComponent<gce::UiImage>();
	dammageUIImage->rectImage = new gce::RectanglePosF(0.0f, 0.0f, 1920.0f, 1080.0f);
	dammageUIImage->btmBrush = new gce::BitMapBrush("res/ArenaShooter/UI/DammageShield.png");
	dammageUIImage->InitializeImage({ 0.0f, 0.0f }, { 1920.0f, 1080.0f }, 0.0f);
}