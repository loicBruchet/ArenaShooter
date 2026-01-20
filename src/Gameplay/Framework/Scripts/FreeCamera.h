#pragma once

#include <Engine.h>

/// <summary>
/// This is a debug script that allows free camera movement using ZQSD keys and mouse.
/// </summary>
DECLARE_SCRIPT(FreeCamera, ScriptFlag::Update)
public:
	virtual void Update() override;

END_SCRIPT