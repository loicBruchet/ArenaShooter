#include "PlayerAnimations.h"

#include <cmath>
#include <algorithm>
#include <Audio.h>

#include "Framework/ResourceManager.h"

#include "Game/Scripts/Weapon/Weapon.h"
#include "Game/Scripts/Player/PlayerCamera.h"

void PlayerAnimations::Start() {
	m_movements = m_pOwner->GetScript<PlayerMovements>();

	m_camera = m_pOwner->GetChildren()[0];
	m_weapon = m_camera->GetChildren()[0];

	if (m_weapon == nullptr)
		return;

	m_weaponOriginalPos = m_weapon->transform.GetLocalPosition();

	if(m_camera == nullptr)
		return;

	m_cameraComponent = m_camera->GetComponent<gce::Camera>();

	m_weapon->GetScript<Weapon>()->OnShoot.AddListener(&PlayerAnimations::OnWeaponShoot, this);
	m_weapon->GetScript<Weapon>()->OnReloadStarted.AddListener(&PlayerAnimations::OnWeaponReload, this);
}

float PlayerAnimations::Sanitize(float v, float fallback) {
	if (!std::isfinite(v))
		return fallback;
	return v;
}

//////////////// WEAPON ANIMATIONS ////////////////

void PlayerAnimations::OnWeaponShoot() {
	gce::AudioUse::Play(RESOURCE_MANAGER.GetSound("ShotgunShoot"), false, m_pOwner->transform.GetWorldPosition());
	PlayerCamera::Shake(0.1f, 0.25f);
	m_isWeaponShooting = true;
}

void PlayerAnimations::OnWeaponReload() {
	gce::AudioUse::Play(RESOURCE_MANAGER.GetSound("ShotgunReload"), false, m_pOwner->transform.GetWorldPosition());
	gce::AudioUse::SetVolumeOfSound("ShotgunReload", 35);
	m_isWeaponReloading = true;
}

const TransformAnimDesc& PlayerAnimations::ComputeWeaponSway(float _dt) {
	gce::Vector2f32 mouse_delta = gce::GameManager::GetMouseDelta();
	mouse_delta /= (_dt + 0.001f); // Remove frame time dependency
	mouse_delta *= 0.01f; // Sensitivity

	static gce::Vector2f32 smoothed = { 0.0f, 0.0f };

	const float smoothing_speed = 10.0f;
	float smooth = 1.0f - std::exp(-smoothing_speed * _dt);

	smoothed.x += (mouse_delta.x - smoothed.x) * smooth;
	smoothed.y += (mouse_delta.y - smoothed.y) * smooth;

	smoothed.x = Sanitize(smoothed.x, 0.0f);
	smoothed.y = Sanitize(smoothed.y, 0.0f);

	// Clamp the delta values to avoid extreme rotations
	smoothed.x = std::clamp(smoothed.x, -50.0f, 50.0f);
	smoothed.y = std::clamp(smoothed.y, -50.0f, 50.0f);

	static TransformAnimDesc desc;
	desc.pos_offset = { 0.0f, 0.0f, 0.0f };
	desc.rot_offset = { smoothed.y / 256.0f, smoothed.x / 256.0f, 0.0f };
	return desc;
}
const TransformAnimDesc& PlayerAnimations::ComputeWeaponCrouch(float _dt) {
	static float crouch_rotation = 0.0f;
	static float crouch_movement = 0.0f;

	const bool crouching = m_movements->IsCrouching();

	if (crouching) {
		crouch_rotation = std::lerp(crouch_rotation, 0.5f, _dt * 10.0f);
		crouch_movement = std::lerp(crouch_movement, -0.025f, _dt * 12.0f);
	}
	else {
		crouch_rotation = std::lerp(crouch_rotation, 0.0f, _dt * 10.0f);
		crouch_movement = std::lerp(crouch_movement, 0.0f, _dt * 12.0f);
	}

	static TransformAnimDesc desc;
	desc.pos_offset = { 0.0f, crouch_movement, 0.0f };
	desc.rot_offset = { 0.0f, 0.0f, crouch_rotation };
	return desc;
}
const TransformAnimDesc& PlayerAnimations::ComputeWeaponMove(float _dt, float _t) {
	gce::Vector3f32 velocity = m_movements->GetVelocity();
	const float base_speed = m_movements->GetBaseSpeed();

	const float horizontal_speed = std::sqrt(velocity.x * velocity.x + velocity.z * velocity.z);
	float targetBlend = 0.0f;
	if (base_speed > 0.0001f) {
		targetBlend = horizontal_speed / base_speed;
		if (targetBlend > 1.0f) targetBlend = 1.0f;
	}

	static float current_blend = 0.0f;
	const float blend_smooth = 8.0f; // Higher value = faster
	current_blend = std::lerp(current_blend, targetBlend, _dt * blend_smooth);

	static float animation_speed = 4.0f;
	static float animation_intensity = 0.025f * 0.75f;

	// Change the animation speed & intensity if we are running
	if(m_movements->IsRunning()) {
		animation_speed = 6.0f;
		animation_intensity = 0.035f * 0.75f;
	}
	else {
		animation_speed = 4.0f;
		animation_intensity = 0.025f * 0.75f;
	}

	float horizontal_movement = std::sin((gce::PI / 2.0f) * _t * animation_speed) * animation_intensity * current_blend;
	float vertical_movement = -std::sin((gce::PI / 2.0f) + (gce::PI * _t) * animation_speed) * animation_intensity * 0.35f * current_blend;
	float forward_rotation = -std::sin((gce::PI * _t) * animation_speed) * animation_intensity * 1.5f * current_blend;

	static TransformAnimDesc desc;
	desc.pos_offset = { horizontal_movement, vertical_movement, 0.0f };
	desc.rot_offset = { 0.0f, 0.0f, forward_rotation };
	return desc;
}
const TransformAnimDesc& PlayerAnimations::ComputeWeaponReload(float _dt, float _t) {
	auto interpolation_function = [](float _time) { // between 0 and 40 (40 is default estimated duration)
		return (5 * _time) - 7 * (std::pow(_time, 2) / 20.0f);
	};

	static bool reloading = false;
	static float reload_anim_time = 0.0f;

	static gce::Vector3f32 reload_rot_offset = { 0.0f, 0.0f, 0.0f };

	if (m_isWeaponReloading && !reloading) {
		reloading = true;
		reload_anim_time = 0.0f;
		m_isWeaponReloading = false;
	}

	if (reloading) {
		reload_anim_time += _dt * 64.0f;

		float interp = interpolation_function(reload_anim_time);

		float to_rad = gce::PI / 180.0f;

		float interp_in_rad = interp * to_rad;

		reload_rot_offset.x = interp_in_rad;

		if (reload_anim_time >= 40.0f) {
			reloading = false;
			reload_anim_time = 0.0f;
			reload_rot_offset = { 0.0f, 0.0f, 0.0f };
		}
	}

	static TransformAnimDesc desc;
	desc.pos_offset = { 0.0f, 0.0f, 0.0f };
	desc.rot_offset = reload_rot_offset;
	return desc;
}
const TransformAnimDesc& PlayerAnimations::ComputeWeaponShoot(float _dt, float _t) {
	static gce::Vector3f32 pos_offset = { 0.0f, 0.0f, 0.0f };
	static gce::Vector3f32 rot_offset = { 0.0f, 0.0f, 0.0f };
	static gce::Vector3f32 pos_vel = { 0.0f, 0.0f, 0.0f };
	static gce::Vector3f32 rot_vel = { 0.0f, 0.0f, 0.0f };

	// WARNING : This is stolen from stackoverflow XD

	// Spring parameters
	const float pos_frequency = 6.0f;      // Lower = more dramatic bounce
	const float pos_damping_ratio = 0.4f;  // Less damping = more bounce
	const float rot_frequency = 6.0f;      // Lower = longer, more visible recovery
	const float rot_damping_ratio = 0.35f; // Less damping = more dramatic overshoot

	// Recoil impulse
	const gce::Vector3f32 recoil_pos = { 0.0f, 0.0f, -20.0f };   // Strong up and back
	const gce::Vector3f32 recoil_rot = { -320.0f, 0.0f, 0.0f };  // Powerful upward kick

	// Trigger recoil on shoot
	if (m_isWeaponShooting) {
		pos_vel += recoil_pos;
		rot_vel += recoil_rot;
		// Add random variation for organic feel
		rot_vel.z += ((std::rand() % 200 - 100) / 100.0f) * 8.0f;
		rot_vel.y += ((std::rand() % 200 - 100) / 100.0f) * 5.0f;
		m_isWeaponShooting = false;
	}

	// Spring damp physics
	auto update_spring = [_dt](gce::Vector3f32& offset, gce::Vector3f32& velocity, 
	                            float frequency, float damping_ratio) {
		float omega = 2.0f * gce::PI * frequency;
		float zeta = damping_ratio;
		
		float k = omega * omega;           // Stiffness
		float c = 2.0f * zeta * omega;     // Damping

		gce::Vector3f32 spring_force = offset * (-k) + velocity * (-c);
		
		velocity += spring_force * _dt;

		offset += velocity * _dt;
	};

	// Update both springs
	update_spring(pos_offset, pos_vel, pos_frequency, pos_damping_ratio);
	update_spring(rot_offset, rot_vel, rot_frequency, rot_damping_ratio);

	// Threshold to snap to zero when settled (prevents endless micro-oscillations)
	auto should_reset = [](const gce::Vector3f32& offset, const gce::Vector3f32& velocity, 
	                       float pos_threshold, float vel_threshold) -> bool {
		float pos_mag_sq = offset.x * offset.x + offset.y * offset.y + offset.z * offset.z;
		float vel_mag_sq = velocity.x * velocity.x + velocity.y * velocity.y + velocity.z * velocity.z;
		return pos_mag_sq < (pos_threshold * pos_threshold) && vel_mag_sq < (vel_threshold * vel_threshold);
	};

	if (should_reset(pos_offset, pos_vel, 0.001f, 0.01f)) {
		pos_offset = { 0.0f, 0.0f, 0.0f };
		pos_vel = { 0.0f, 0.0f, 0.0f };
	}
	
	if (should_reset(rot_offset, rot_vel, 0.1f, 0.5f)) {
		rot_offset = { 0.0f, 0.0f, 0.0f };
		rot_vel = { 0.0f, 0.0f, 0.0f };
	}

	float to_rad = gce::PI / 180.0f;

	static TransformAnimDesc desc;
	desc.pos_offset = pos_offset;
	desc.rot_offset = { rot_offset.x * to_rad, rot_offset.y * to_rad, rot_offset.z * to_rad };
	return desc;
}

void PlayerAnimations::ApplyWeaponAnimations(float _dt, float _t) {
	// Compute animations
	const TransformAnimDesc& move_anim = ComputeWeaponMove(_dt, _t);
	const TransformAnimDesc& crouch_anim = ComputeWeaponCrouch(_dt);
	const TransformAnimDesc& sway_anim = ComputeWeaponSway(_dt);
	const TransformAnimDesc& shoot_anim = ComputeWeaponShoot(_dt, _t);
	const TransformAnimDesc& reload_anim = ComputeWeaponReload(_dt, _t);

	// Apply animations
	m_weapon->transform.SetLocalPosition(m_weaponOriginalPos + move_anim.pos_offset + crouch_anim.pos_offset + shoot_anim.pos_offset);
	m_weapon->transform.SetLocalRotation(move_anim.rot_offset + sway_anim.rot_offset + crouch_anim.rot_offset);
	m_weapon->transform.LocalRotate(reload_anim.rot_offset + shoot_anim.rot_offset);
}

///////////////////////////////////////////////////

//////////////// CAMERA ANIMATIONS ////////////////

const float PlayerAnimations::ComputeCameraFOV(float _dt) {
	static float base_fov = m_cameraComponent->perspective.fov;
	bool isRunning = m_movements->IsRunning();
	const float target_fov = isRunning ? (base_fov + gce::PI / 15.0f) : base_fov;

	// blend between base fov and running fov
	static float current_fov = base_fov;
	const float fov_smooth = 5.0f; // Higher value = faster
	current_fov = std::lerp(current_fov, target_fov, _dt * fov_smooth);
	return current_fov;
}

void PlayerAnimations::ApplyCameraAnimations(float _dt, float _t) {
	// Compute animations
	const float fov_anim = ComputeCameraFOV(_dt);

	// Apply animations
	m_cameraComponent->perspective.fov = fov_anim;
}

///////////////////////////////////////////////////

void PlayerAnimations::Update() {
	static float time = 0.0f;
	float deltaTime = gce::GameManager::DeltaTime();
	time += deltaTime;
	ApplyWeaponAnimations(deltaTime, time);
	ApplyCameraAnimations(deltaTime, time);
}

