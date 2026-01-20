#pragma once

#include <Engine.h>

#include "PlayerMovements.h"

struct TransformAnimDesc {
	gce::Vector3f32 pos_offset;
	gce::Vector3f32 rot_offset;
};

DECLARE_SCRIPT(PlayerAnimations, ScriptFlag::Start | ScriptFlag::Update)
public:
	virtual void Start() override;
	virtual void Update() override;

	float Sanitize(float v, float fallback = 0.0f);

//////////////// WEAPON ANIMATIONS ////////////////
private:
	bool m_isWeaponShooting = false;
	bool m_isWeaponReloading = false;
	gce::GameObject* m_weapon = nullptr;
	gce::Vector3f32 m_weaponOriginalPos = { 0.0f, 0.0f, 0.0f };
	void OnWeaponShoot();
	void OnWeaponReload();

	void ApplyWeaponAnimations(float _dt, float _t);
	const TransformAnimDesc& ComputeWeaponSway(float _dt);
	const TransformAnimDesc& ComputeWeaponCrouch(float _dt);
	const TransformAnimDesc& ComputeWeaponMove(float _dt, float _t);
	const TransformAnimDesc& ComputeWeaponShoot(float _dt, float _t);
	const TransformAnimDesc& ComputeWeaponReload(float _dt, float _t);

///////////////////////////////////////////////////

//////////////// CAMERA ANIMATIONS ////////////////
private:
	gce::GameObject* m_camera = nullptr;
	gce::Camera* m_cameraComponent = nullptr;

	void ApplyCameraAnimations(float _dt, float _t);
	const float ComputeCameraFOV(float _dt);

private:
	PlayerMovements* m_movements = nullptr;

END_SCRIPT