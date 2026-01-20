#include "PlayerMovements.h"

#include <cmath>
#include <algorithm>

#include "Game/Utils/ColliderUtils.h"
#include "Game/Tags.h"

void PlayerMovements::Start() {
	if (!m_pOwner) return;

	if (!physics) { physics = m_pOwner->GetComponent<gce::PhysicComponent>(); }
	if (!collider) { collider = m_pOwner->GetComponent<gce::BoxCollider>(); }

	if (collider) {
		if (collider->localScale.y <= 0.001f) {
			collider->localScale = { 1.0f, 2.0f, 1.0f };
		}
	}

	auto const& children = m_pOwner->GetChildren();
	if (children.size() > 1) {
		groundCheckerObject = children[1];
		rooftopCheckerObject = children[2];
		groundChecker = groundCheckerObject->GetComponent<gce::SphereCollider>();
		rooftopChecker = rooftopCheckerObject->GetComponent<gce::SphereCollider>();
	}

	if (groundChecker) {
		groundChecker->collisionStayEvent.AddListener(&PlayerMovements::OnGroundCollisionEnter, this);
		groundChecker->collisionExitEvent.AddListener(&PlayerMovements::OnGroundCollisionExit, this);
	}
	if (rooftopChecker) {
		rooftopChecker->collisionEnterEvent.AddListener(&PlayerMovements::OnRoofTopCollisionEnter, this);
		rooftopChecker->collisionExitEvent.AddListener(&PlayerMovements::OnRoofTopCollisionExit, this);
	}
	isGrounded = true;
}

void PlayerMovements::Update() {

	if (!m_pOwner || !physics || !collider) return;

	// Debug cursor lock/unlock, TO DO : remove this later
	if (gce::GetKeyDown(gce::Keyboard::B)) {
		gce::GameManager::UnlockCursor();
	}
	if (gce::GetKeyDown(gce::Keyboard::F)) {
		gce::GameManager::LockCursor();
	}

	const float dt = gce::GameManager::DeltaTime();
	gce::Vector3f32 velocity = physics->GetVelocity();

	bool inputPressed = false;
	bool isMoving = false;

	const gce::Vector3f32 forward = m_pOwner->transform.GetLocalForward();
	const gce::Vector3f32 right = m_pOwner->transform.GetLocalRight();

	auto applyInput = [&](bool condition, const gce::Vector3f32& dir) {
		if (isSliding) return;
		if (!condition) return;
		velocity += dir * acceleration * 1024.0f * dt;
		inputPressed = true;
		isMoving = true;
	};

	applyInput(gce::GetKey(gce::Keyboard::Z), forward);
	applyInput(gce::GetKey(gce::Keyboard::S), -forward);
	applyInput(gce::GetKey(gce::Keyboard::Q), -right);
	applyInput(gce::GetKey(gce::Keyboard::D), right);

	bool canRun = true;
	float boostSpeed = 1.0f;

	if (gce::GetKey(gce::Keyboard::LCONTROL) && isGrounded) {
		if (!isCrouching) {
			collider->localScale = { 1.0f, 1.0f, 1.0f };
			m_pOwner->transform.LocalTranslate({ 0.0f, -1.0f, 0.0f });
		}
		isCrouching = true;

		canRun = false;
	}
	else {
		if (!isRoofTop) {
			rooftopCheckerObject->transform.SetLocalPosition({ 0.0f, 1.5f, 0.0f });
			groundCheckerObject->transform.SetLocalPosition({ 0.0f, 0.0f, 0.0f });
			collider->localScale = { 1.0f, 2.0f, 1.0f };
			isCrouching = false;
		}
	}

	if (isCrouching) {
		boostSpeed = 0.25f;
	}

	if (gce::GetKeyDown(gce::Keyboard::LCONTROL)) {
		if (isGrounded) {

			rooftopCheckerObject->transform.SetLocalPosition({ 0.0f, 2.0f, 0.0f });
			groundCheckerObject->transform.SetLocalPosition({ 0.0f, 0.5f, 0.0f });
		}

		if (!isSliding && isGrounded && gce::GetKey(gce::Keyboard::LSHIFT) && isMoving) {
			const float32 currHorizSpeed = gce::Vector2f32(velocity.x, velocity.z).Norm();

			initialSlideSpeed = currHorizSpeed + slideImpulse;

			slideDirection = gce::Vector2f32(velocity.x, velocity.z);
			if (slideDirection.Norm() <= 0.0001f) {
				slideDirection = gce::Vector2f32(forward.x, forward.z);
			}
			slideDirection = slideDirection.Normalize();

			velocity.x = slideDirection.x * initialSlideSpeed;
			velocity.z = slideDirection.y * initialSlideSpeed;

			slideTimer = 0.0f;
			isSliding = true;
		}
	}

	if (!gce::GetKey(gce::Keyboard::LCONTROL) || !isGrounded) {
		isSliding = false;
	}

	const float32 maxBaseSpeed = GetBaseSpeed();
	if (isSliding) {
		slideTimer += dt;
		if (slideTimer < 0.0f) slideTimer = 0.0f;
		float t = slideTimer / slideDuration;
		if (t > 1.0f) t = 1.0f;

		const float32 targetRunSpeed = maxBaseSpeed * runSpeed;

		// Cubic ese out: https://easings.net/fr (yep i stoll it)
		const float32 ease = 1.0f - std::powf(1.0f - t, 3.0f);

		// Interpolate between initialSlideSpeed and targetRunSpeed using the ease value
		const float32 desiredSpeed = initialSlideSpeed + (targetRunSpeed - initialSlideSpeed) * ease;

		velocity.x = slideDirection.x * desiredSpeed;
		velocity.z = slideDirection.y * desiredSpeed;

		if (t >= 1.0f) {
			isSliding = false;
		}
	}

	if (!inputPressed && isGrounded && !isSliding) {
		float decelerationFactor = 1.0f - (deceleration * 64.0f * dt);
		if (decelerationFactor < 0.0f) decelerationFactor = 0.0f;
		velocity.x *= decelerationFactor;
		velocity.z *= decelerationFactor;
	}

	float32 speed = velocity.Norm();
	if (speed > maxBaseSpeed) {
		if (!isSliding) {
			if (gce::GetKey(gce::Keyboard::LSHIFT) && canRun && isMoving) {
				isRunning = true;
				boostSpeed = runSpeed;
			}
			else {
				isRunning = false;
				//boostSpeed = 1.0f;
			}
			gce::Vector2f32 horizontal = gce::Vector2f32(velocity.x, velocity.z).Normalize() * maxBaseSpeed * boostSpeed;
			velocity = gce::Vector3f32(horizontal.x, velocity.y, horizontal.y);
		}
	}

	float currentPhysY = physics->GetVelocity().y;
	velocity.y = currentPhysY;

	bool jumpRequested = gce::GetKeyDown(gce::Keyboard::SPACE) && (currentPhysY == 0.0f || isGrounded);
	if (jumpRequested) {
		if (!inputPressed) {
			velocity.x = 0.0f;
			velocity.z = 0.0f;
		}
		velocity.y += jumpForce;
		// Cancel slide on jump
		isSliding = false;
	}

	physics->SetVelocity(velocity);
}

void PlayerMovements::OnGroundCollisionEnter(gce::GameObject* other) {
	if (other->HasTag(TAG_OTHER))
		return;

	if (IsTrigger(other))
		return;

	isGrounded = true;
}

void PlayerMovements::OnGroundCollisionExit(gce::GameObject* other) {
	if (other->HasTag(TAG_OTHER))
		return;

	if (IsTrigger(other))
		return;

	isGrounded = false;
}

void PlayerMovements::OnRoofTopCollisionEnter(gce::GameObject* other) {
	if (other->HasTag(TAG_OTHER))
		return;

	if (IsTrigger(other))
		return;

	isRoofTop = true;
}

void PlayerMovements::OnRoofTopCollisionExit(gce::GameObject* other) {
	if (other->HasTag(TAG_OTHER))
		return;

	if (IsTrigger(other))
		return;

	isRoofTop = false;
}

// ANIMATION API
gce::Vector3f32 PlayerMovements::GetVelocity() const {
	if (!physics) return { 0.0f, 0.0f, 0.0f };
	return physics->GetVelocity();
}

const float32 PlayerMovements::GetBaseSpeed() const {
	return walkSpeed * 10.0f;
}

const bool PlayerMovements::IsCrouching() const {
	return isCrouching;
}
const bool PlayerMovements::IsRunning() const {
	return isRunning;
}