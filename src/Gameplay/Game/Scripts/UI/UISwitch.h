#pragma once

#include <Engine.h>


DECLARE_SCRIPT(UISwitch, ScriptFlag::Start | ScriptFlag::Update)
public:
	
	float ScaleFactor = 1.0f;
	virtual void Start() override;
	virtual void Update() override;

	void ChangeActive(bool _active);
private:

	bool IsActive = true;
	END_SCRIPT