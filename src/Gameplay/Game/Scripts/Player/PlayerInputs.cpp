#include "PlayerInputs.h"

#include <Engine.h>

#include "PlayerCamera.h"

void PlayerInputs::Start() {
	m_ammoText = m_pOwner->GetChildren()[3]->GetComponent<gce::TextRenderer>();
	m_movements = m_pOwner->GetScript<PlayerMovements>();
	m_weapon = m_pOwner->GetChildren()[0]->GetChildren()[0]->GetScript<Weapon>();
}

void PlayerInputs::Update() {
	if (m_weapon == nullptr || m_ammoText == nullptr)
		return;

	if (gce::GetButtonDown(gce::Mouse::LEFT)) {
		m_weapon->TryShoot();
	}

	if (gce::GetKeyDown(gce::Keyboard::R))
	{
		m_weapon->TryReload();
	}
	if (gce::GetKeyDown(gce::Keyboard::F))
	{
		if (m_weapon->bulletPrefabName == "ShotgunProjectilePrefab")
		{
			m_weapon->bulletPrefabName = "GuidedShotgunProjectilePrefab";
		}
		else
		{
			m_weapon->bulletPrefabName = "ShotgunProjectilePrefab";
		}
	}
	if (gce::GetKeyDown(gce::Keyboard::I)) {
		gce::Scene& scene = gce::Scene::Create();
		gce::GameManager::ChangeScene(&scene);
	}

	m_ammoText->text = std::to_wstring(m_weapon->currentAmmo) + std::wstring(L"/") + std::to_wstring(m_weapon->maxAmmo);
}