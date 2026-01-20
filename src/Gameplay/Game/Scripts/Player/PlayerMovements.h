#pragma once

#include <Engine.h>

DECLARE_SCRIPT(PlayerMovements, ScriptFlag::Start | ScriptFlag::Update)
public:
	const float32 acceleration = 0.8f; // The speed at wich the player gains speed (do i realy need to explain this Loïc ?)
	const float32 deceleration = 0.5f; // Same as above but for losing speed (rocket science right here)

	const float32 walkSpeed = 1.0f;
	const float32 runSpeed = 2.0f;
	const float32 crouchSpeed = 0.5f;

	const float32 jumpForce = 15.0f;

	// Slide configuration
	const float32 slideDuration = 0.8f; // Duration of the slide in seconds, before getting back to run speed
	const float32 slideImpulse = 80.0f; // Initial impulse force applied when starting a slide (again, no unit XD)

	virtual void Start() override;
	virtual void Update() override;

// ANIMATION API
public:
	gce::Vector3f32 GetVelocity() const;
	const float32 GetBaseSpeed() const;
	const bool IsCrouching() const;
	const bool IsRunning() const;

private:
	gce::SphereCollider* groundChecker = nullptr;
	gce::SphereCollider* rooftopChecker = nullptr;
	gce::PhysicComponent* physics = nullptr;
	gce::BoxCollider* collider = nullptr;

	gce::GameObject* groundCheckerObject = nullptr;
	gce::Vector3f32 groundCheckerLocalPos {};
	gce::GameObject* rooftopCheckerObject = nullptr;
	gce::Vector3f32 rooftopCheckerLocalPos {};

	void OnGroundCollisionEnter(gce::GameObject* other);
	void OnGroundCollisionExit(gce::GameObject* other);
	void OnRoofTopCollisionEnter(gce::GameObject* other);
	void OnRoofTopCollisionExit(gce::GameObject* other);

	bool isCrouching = false;
	bool isGrounded = false;
	bool isRoofTop = false;
	bool isRunning = false;

	// Sliding state
	bool isSliding = false;
	float32 slideTimer = 0.0f;
	float32 initialSlideSpeed = 0.0f;
	gce::Vector2f32 slideDirection {}; // locked direction for the slide

END_SCRIPT