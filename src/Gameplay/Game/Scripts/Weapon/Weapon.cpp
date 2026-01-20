#include "Weapon.h"

#include "Framework/PrefabRegistry.h"
#include "Framework/Prefab.h"

void Weapon::Init(int _maxAmmo, float _fireRate, float _reloadTime, gce::GameObject* _spawnpoint) {
	maxAmmo = _maxAmmo;
	currentAmmo = maxAmmo;
	fireRate = _fireRate;
	reloadTime = _reloadTime;
	// fireTimer = Timer(1 / fireRate);
	spawnpoint = _spawnpoint;
}

void Weapon::Destroy() {
	OnShoot.ClearListeners();
	OnReloadStarted.ClearListeners();
	OnReloadEnded.ClearListeners();

	delete fireTimer;
}

void Weapon::Start() {
	fireTimer->Start();
	reloadTimer->OnCompleted.AddListener(&Weapon::Reload, this);
	scene = gce::GameManager::GetActiveScene();
}

void Weapon::Update() {
	fireTimer->SetDuration(1 / fireRate);
	reloadTimer->SetDuration(reloadTime);

	if (fireTimer->IsCompleted())
		canShoot = true;
		// fireTimer = Timer(1 / fireRate);
}

void Weapon::TryShoot() {
	if (!canShoot)
		return;

	if (currentAmmo <= 0)
		return;

	Shoot();
	canShoot = false;
	fireTimer->Restart();
}


void Weapon::TryReload() {
	if (currentAmmo >= maxAmmo)
		return;

	if (reloadTimer->IsRunning())
		return;

	reloadTimer->Restart();
	OnReloadStarted.Invoke();
}

void Weapon::Shoot() {
	Prefab* projectile = PREFAB_REGISTRY.CreatePrefab(bulletPrefabName, *scene);

	gce::GameObject& object = projectile->GetRootObject();

	if (spawnpoint == nullptr) {
		object.transform.SetWorldPosition(m_pOwner->transform.GetWorldPosition() + m_pOwner->transform.GetWorldForward());
		object.transform.SetWorldRotation(m_pOwner->transform.GetWorldRotation());
	}
	else {
		object.transform.SetWorldPosition(spawnpoint->transform.GetWorldPosition());
		object.transform.SetWorldRotation(spawnpoint->transform.GetWorldRotation());
	}

	OnShoot.Invoke();

	currentAmmo--;
}

void Weapon::Reload() {
	currentAmmo = maxAmmo;
	OnReloadEnded.Invoke();
}