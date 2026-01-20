#pragma once

#include <Engine.h>
#include "NavGrid.h"

class Grid;
class Path;

class Cell;

DECLARE_SCRIPT(Agent, ScriptFlag::Start | ScriptFlag::Update)
public:
	float movementSpeed = 5.0f; // Units per second
	float rotationSpeed = 10.0f; // Radians per second

	float stoppingDistance = 1.0f; // Distance to target to consider arrived

	virtual void Start() override;
	virtual void Update() override;

	void GoTo(const gce::Vector3f32& _target);

private:
	NavGrid* m_navgrid = nullptr;

	void GoToPosition(gce::Vector3f32 _target);
	void RotateTowards(gce::Vector3f32 _target);

	gce::Vector3f32 m_target;
	std::vector<GridPosition> m_path;

END_SCRIPT