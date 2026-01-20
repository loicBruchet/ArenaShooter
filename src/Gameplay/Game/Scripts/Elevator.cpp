#include "Elevator.h"

#include "Game/Tags.h"

#include "Game/Scripts/Player/KeyInventory.h"

void Elevator::Start()
{
	m_boxCollider = m_pOwner->GetComponent<gce::BoxCollider>();
	m_boxCollider->collisionStayEvent.AddListener(&Elevator::OnCollisionEnter, this);

	gce::GameObject& radiusVisualizer = gce::GameObject::Create(*gce::GameManager::GetActiveScene());
	radiusVisualizer.SetName("HomingRadiusVisualizer");
	radiusVisualizer.SetParent(*m_pOwner);
	radiusVisualizer.SetTags(TAG_OTHER);
	radiusVisualizer.transform.SetLocalPosition({ 0.0f, 1.5f, 0.0f });
	radiusVisualizer.transform.SetLocalScale({ 1.0f,1.0f,1.0f });


	//gce::MeshRenderer& meshRenderer = *radiusVisualizer.AddComponent<gce::MeshRenderer>();
	//meshRenderer.SetGeometry(gce::SHAPES.CUBE);

	gce::BoxCollider& boxCollider = *radiusVisualizer.AddComponent<gce::BoxCollider>();
	boxCollider.localScale = { 1.0f,1.0f,1.0f };
	boxCollider.isTrigger = true;

	boxCollider.collisionStayEvent.AddListener(&Elevator::OnCollisionEnterUp, this);
		
	boxCollider.collisionExitEvent.AddListener(&Elevator::OnCollisionExitUp, this);
}
void Elevator::OnCollisionEnter(gce::GameObject* other)
{
	if (m_hasNotCollide)
		return;
	if (other->HasTag(TAG_PLAYER) && KeyInventory::HasKey())
	{
		gce::Vector3f32 up = m_pOwner->transform.GetLocalUp();
		gce::Vector3f32 velocity = up * gce::GameManager::DeltaTime() * m_speed;
		m_pOwner->transform.LocalTranslate(velocity);
	}
}
void Elevator::OnCollisionEnterUp(gce::GameObject* other)
{
	m_hasNotCollide = false;
}
void Elevator::OnCollisionExitUp(gce::GameObject* other)
{
	m_hasNotCollide = true;
}
