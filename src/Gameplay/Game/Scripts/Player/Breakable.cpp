#include "Breakable.h"
#include "Game/Tags.h"
void Breakable::Start() {
	m_healthStat = m_pOwner->GetScript<HealthStat>();
	if (m_healthStat) {
		m_healthStat->OnTakeDammage.AddListener(&Breakable::OnTakeDammage, this);
	}
}

void Breakable::OnTakeDammage(gce::GameObject* _gm) {
	m_pOwner->Destroy();
}