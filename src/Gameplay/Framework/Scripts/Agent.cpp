#include "Agent.h"

void Agent::Start() {
	gce::Scene* scene = gce::GameManager::GetActiveScene();
	gce::GameObject* object = gce::GameManager::GetObjectByName("NavGridObjet");

	m_navgrid = object->GetScript<NavGrid>();
}

void Agent::Update() {
	// Make sure the agent do not move to close to the target
	gce::Vector3f32 delta = m_target - m_pOwner->transform.GetWorldPosition();
	if (delta.Norm() < stoppingDistance)
		return;

	// Move along the path
	if (m_path.empty())
		return;

	GridPosition nextPos = m_path.front();
	gce::Vector3f32 targetPos = m_navgrid->ToWorldPosition(nextPos);
	targetPos.y = m_pOwner->transform.GetWorldPosition().y;
	if ((targetPos - m_pOwner->transform.GetWorldPosition()).Norm() < 0.1f) {
		m_path.erase(m_path.begin());
		return;
	}

	GoToPosition(targetPos);
	RotateTowards(targetPos);
}

void Agent::GoToPosition(gce::Vector3f32 _target) {
	gce::Vector3f32 currentPosition = m_pOwner->transform.GetWorldPosition();
	gce::Vector3f32 direction = _target - currentPosition;
	direction.SelfNormalize();
	gce::Vector3f32 newPosition = currentPosition + direction * movementSpeed * gce::GameManager::DeltaTime();
	m_pOwner->transform.SetWorldPosition(newPosition);
}

void Agent::RotateTowards(gce::Vector3f32 _target) {
	gce::Vector3f32 direction = _target - m_pOwner->transform.GetWorldPosition();
	direction.SelfNormalize();

	gce::Quaternion current_rotation = m_pOwner->transform.GetWorldRotation();
	gce::Quaternion target_rotation = gce::Quaternion::RotationAxis({ 0.0f, 1.0f, 0.0f }, atan2f(direction.x, direction.z));

	current_rotation.Lerp(target_rotation, rotationSpeed * gce::GameManager::DeltaTime());
	m_pOwner->transform.SetWorldRotation(current_rotation);
}

void Agent::GoTo(const gce::Vector3f32& _target) {
	// Make sure the target is at the same height as the agent
	gce::Vector3f32 target = _target;
	target.y = m_pOwner->transform.GetWorldPosition().y;

	// Make sure to not compute a new path if already close to the target
	gce::Vector3f32 delta = target - m_pOwner->transform.GetWorldPosition();
	if (delta.Norm() < stoppingDistance)
		return;

	m_path = m_navgrid->ComputePath(
		m_navgrid->ToGridPosition(m_pOwner->transform.GetWorldPosition()),
		m_navgrid->ToGridPosition(target)
	);
	if (m_path.empty())
		return;
	m_path.erase(m_path.begin()); // Remove current position
	m_target = target; // Update target
}