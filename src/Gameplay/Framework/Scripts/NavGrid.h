#pragma once

#include <Engine.h>

#include <vector>
#include <queue>
#include <unordered_map>
#include <limits>

struct GridPosition {
	int x;
	int z;
};
struct GridCell {
public:
	gce::Vector2i32 position;
	int index;

	int dist_to_player;
	int dist_to_goal;

	bool visited;
	GridCell* from;
};
class GridCellComparator {
public:
	bool operator()(GridCell* _a, GridCell* _b) {
		int score_a = _a->dist_to_player + _a->dist_to_goal;
		int score_b = _b->dist_to_player + _b->dist_to_goal;
		return score_a > score_b;
	}
};

DECLARE_SCRIPT(NavGrid, ScriptFlag::Start | ScriptFlag::Update)
public:
	int resolutionX = 0;
	int resolutionZ = 0;

	float sizeX = 0.0f;
	float sizeZ = 0.0f;

	float cellSize = 0.0f;

	void Init(char* _data);

	std::vector<GridPosition> ComputePath(GridPosition start, GridPosition end);

	virtual void Start() override;
	virtual void Update() override;

	int ToIndex(const GridPosition& gridPos);
	GridPosition ToGridPosition(const gce::Vector3f32& worldPos);
	gce::Vector3f32 ToWorldPosition(const GridPosition& gridPos);

private:
	char* m_data;

	void SpawnCubesGridForDebug();

END_SCRIPT