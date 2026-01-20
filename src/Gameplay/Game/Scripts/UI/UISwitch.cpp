#include "UISwitch.h"

#include "Framework/ResourceManager.h"

void UISwitch::Start() {

}

void UISwitch::Update() {
	if (gce::GetKeyDown(gce::Keyboard::F)) {
		ChangeActive(!IsActive);
	}
}

void UISwitch::ChangeActive(bool _active)
{
	IsActive = _active;
	if (IsActive)
	{
		m_pOwner->transform.SetLocalScale({ ScaleFactor,ScaleFactor,0.0f });
	}
	else m_pOwner->transform.SetLocalScale({ 0.0f,0.0f,0.0f });
	
}
