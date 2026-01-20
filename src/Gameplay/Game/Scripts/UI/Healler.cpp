#include "Healler.h"

#include "Game/Utils/ColliderUtils.h"
#include "Game/Tags.h"
#include "Game/Scripts/Stats/HealthStat.h"

void Healler::Start() {
	m_collider = m_pOwner->GetComponent<gce::SphereCollider>();

	m_collider->collisionEnterEvent.AddListener(&Healler::OnCollisionEnter, this);

}
void Healler::Update() {
}

void Healler::OnCollisionEnter(gce::GameObject* _other)
{


	if (IsTrigger(_other))
		return;
	if (_other->HasTag(TAG_OTHER))
		return;

	if (!_other->HasTag(TAG_PLAYER))
		return;

	_other->GetScript<HealthStat>()->Heal(healAmount);
	m_collider->collisionEnterEvent.ClearListeners();
	m_pOwner->Destroy();
}
