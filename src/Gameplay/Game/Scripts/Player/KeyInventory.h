#pragma once
#include <Engine.h>

DECLARE_SCRIPT(KeyInventory, ScriptFlag::Start | ScriptFlag::Update)
public:
	bool hasKey = false;
	virtual void Start() override;
	virtual void Update() override;

	static bool HasKey();

private:
	static KeyInventory* instance;
END_SCRIPT