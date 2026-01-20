#include "RecoltKey.h"

#include "Game/Utils/ColliderUtils.h"
#include "Game/Tags.h"
#include "Game/Scripts/Stats/HealthStat.h"
#include "Game/Scripts/Player/PlayerInputs.h"
#include "Game/Scripts/Player/KeyInventory.h"

#include "Framework/PrefabRegistry.h"
#include "Framework/Prefab.h"
void RecoltKey::Start() {
	m_collider = m_pOwner->GetComponent<gce::SphereCollider>();
	scene = gce::GameManager::GetActiveScene();
	m_collider->collisionEnterEvent.AddListener(&RecoltKey::OnCollisionEnter, this);

}
void RecoltKey::Update() {

}

void RecoltKey::OnCollisionEnter(gce::GameObject* _other)
{
	if (IsTrigger(_other))
		return;
	if (_other->HasTag(TAG_OTHER))
		return;

	if (!_other->HasTag(TAG_PLAYER))
		return;

	PREFAB_REGISTRY.CreatePrefab("KeyUIPrefab", *scene);
	_other->GetScript<KeyInventory>()->hasKey = true;
	m_collider->collisionEnterEvent.ClearListeners();
	m_pOwner->Destroy();
}
