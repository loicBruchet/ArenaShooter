#pragma once

#include <Engine.h>
#include "Game/Utils/Timer.h"

DECLARE_SCRIPT(Weapon, ScriptFlag::Start | ScriptFlag::Update | ScriptFlag::Destroy)
public:
	// Shots per second
	// (I think you get the idea Loic XD, but just in case..)
	// (1 = 1 shot per second, 2 = 2 shots per second, etc)
	float fireRate = 5.0f;

	int maxAmmo = 8;
	int currentAmmo = 8;

	float reloadTime = 2.0f; // Time (in seconds) it takes to reload

	std::string bulletPrefabName = "ShotgunProjectilePrefab";
	gce::GameObject* spawnpoint = nullptr; // Where the bullets are spawned from

	gce::Event<> OnShoot;
	gce::Event<> OnReloadStarted;
	gce::Event<> OnReloadEnded;

	void Init(int _maxAmmo = 8, float _fireRate = 1.0f, float _reloadTime = 2.0f, gce::GameObject* _spawnpoint = nullptr);

	virtual void Start() override;
	virtual void Update() override;
	virtual void Destroy() override;

	void TryShoot();
	void TryReload();

private:
	void Shoot();
	void Reload();

private:
	gce::Scene* scene = nullptr;

	bool canShoot = true;
	
	gce::Vector2f32 direction;
	Timer* fireTimer = new Timer(1 / fireRate);
	Timer* reloadTimer = new Timer(reloadTime);

END_SCRIPT