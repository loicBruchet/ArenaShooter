#include "FreeCamera.h"

void FreeCamera::Update() {
	const float moveSpeed = 5.0f;
	const float rotateSpeed = 1.5f;

	float horizontal = 0.0f;
	float vertical = 0.0f;
	float height = 0.0f;

	if (gce::GetKey(gce::Keyboard::Z)) {
		vertical += moveSpeed * gce::GameManager::DeltaTime();
	}
	if (gce::GetKey(gce::Keyboard::S)) {
		vertical -= moveSpeed * gce::GameManager::DeltaTime();
	}
	if (gce::GetKey(gce::Keyboard::Q)) {
		horizontal -= moveSpeed * gce::GameManager::DeltaTime();
	}
	if (gce::GetKey(gce::Keyboard::D)) {
		horizontal += moveSpeed * gce::GameManager::DeltaTime();
	}
	if (gce::GetKey(gce::Keyboard::SPACE)) {
		height += moveSpeed * gce::GameManager::DeltaTime();
	}
	if (gce::GetKey(gce::Keyboard::LCONTROL)) {
		height -= moveSpeed * gce::GameManager::DeltaTime();
	}

	float speed = 1.0f;
	if (gce::GetKey(gce::Keyboard::LSHIFT)) {
		speed = 2.0f;
	}

	if (gce::GetButtonDown(gce::Mouse::RIGHT)) {
		gce::GameManager::LockCursor();
	}
	if (gce::GetButtonUp(gce::Mouse::RIGHT)) {
		gce::GameManager::UnlockCursor();
	}

	if (!gce::GetButton(gce::Mouse::RIGHT)) {
		return;
	}

	gce::Vector3f32 forward_movement = m_pOwner->transform.GetWorldForward() * vertical;
	gce::Vector3f32 right_movement = m_pOwner->transform.GetWorldRight() * horizontal;
	gce::Vector3f32 up_movement = m_pOwner->transform.GetWorldUp() * height;

	m_pOwner->transform.LocalTranslate((forward_movement + right_movement + up_movement) * speed);

	gce::Vector2f32 deltaMousePos = gce::GameManager::GetMouseDelta();

	static float pitch = 0.0f;
	pitch += deltaMousePos.y / 512.0f;

	static float yaw = 0.0f;
	yaw += deltaMousePos.x / 512.0f;

	m_pOwner->transform.SetWorldRotation({ pitch , yaw, 0.0f });
}