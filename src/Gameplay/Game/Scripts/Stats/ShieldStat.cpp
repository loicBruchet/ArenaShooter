#include "ShieldStat.h"

void ShieldStat::Init(int _maxShield) {
	maxShield = _maxShield;
	currentShield = maxShield;
}

void ShieldStat::Break() {
	broken = true;
	currentShield = 0;
	OnBreak.Invoke(m_pOwner);
}

void ShieldStat::Destroy() {
	OnBreak.ClearListeners();
	OnShieldChanged.ClearListeners();
	OnTakeDammage.ClearListeners();
	OnRegenerate.ClearListeners();

	health = nullptr;
}

void ShieldStat::Start() {
	delayTimer.Start();
	delayTimer.OnCompleted.AddListener(&ShieldStat::StartRegeneration, this);
	rateTimer.OnCompleted.AddListener(&ShieldStat::RegenerateOnRateTimer, this);
	health = m_pOwner->GetScript<HealthStat>();
}

void ShieldStat::Update() {
	if(broken)
		return;
}

void ShieldStat::RegenerateOnRateTimer() {
	Regenerate(1);

	OnRegenerate.Invoke(m_pOwner);
	OnShieldChanged.Invoke(m_pOwner);

	if(canRegenerate && currentShield < maxShield)
		rateTimer.Restart();
}

void ShieldStat::Regenerate(int _ammount) {
	currentShield += _ammount;
	if (currentShield >= maxShield)
		currentShield = maxShield;
}

void ShieldStat::TakeDammage(int _ammount) {
	if (currentShield - _ammount < 0) {
		int overflow = _ammount - currentShield;
		currentShield = 0;
		health->TakeDammage(overflow);
		if(!broken) Break();
		delayTimer.Restart();
		OnTakeDammage.Invoke(m_pOwner);
		OnShieldChanged.Invoke(m_pOwner);
		return;
	}

	currentShield -= _ammount;

	delayTimer.Restart();

	OnTakeDammage.Invoke(m_pOwner);
	OnShieldChanged.Invoke(m_pOwner);
}

const bool ShieldStat::IsBroken() const {
	return broken;
}

void ShieldStat::StartRegeneration()
{
	canRegenerate = true;
	if (canRegenerate && currentShield < maxShield)
		rateTimer.Restart();
}