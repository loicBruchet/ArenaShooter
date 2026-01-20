#include "HealthStat.h"
#include "UIGestion.h"

void HealthStat::Init(int _maxHealth) {
	maxHealth = _maxHealth;
	currentHealth = maxHealth;
}
void HealthStat::Die(bool _destroy) {
	alive = false;
	currentHealth = 0;
	OnDeath.Invoke(m_pOwner);
	if (_destroy)
		Destroy();
}

void HealthStat::Start() {
	collider = m_pOwner->GetComponent<gce::SphereCollider>();
}
void HealthStat::Update() {}

void HealthStat::Destroy() {
	if (collider != nullptr) {
		collider->collisionEnterEvent.ClearListeners();
		collider->collisionStayEvent.ClearListeners();
		collider->collisionExitEvent.ClearListeners();
	}

	collider = nullptr;
}

void HealthStat::Heal(int _ammount) {
	currentHealth += _ammount;

	if (currentHealth >= maxHealth)
		currentHealth = maxHealth;

	OnHeal.Invoke(m_pOwner);
	OnHealthChanged.Invoke(m_pOwner);
}

void HealthStat::TakeDammage(int _ammount) {
	currentHealth -= _ammount;

	if (currentHealth <= 0)
		Die();

	OnTakeDammage.Invoke(m_pOwner);
	OnHealthChanged.Invoke(m_pOwner);
}

const bool HealthStat::IsAlive() const {
	return alive;
}