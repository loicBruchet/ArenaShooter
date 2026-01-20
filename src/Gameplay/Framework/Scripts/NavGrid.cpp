#include "NavGrid.h"

#include <queue>

// Undefine min and max to avoid conflicts
#undef min
#undef max

void NavGrid::Start() {}
void NavGrid::Update() {}

void NavGrid::Init(char* _data) {
	m_data = _data;
}

void NavGrid::SpawnCubesGridForDebug() {
	gce::Scene* scene = gce::GameManager::GetActiveScene();

	for (int z = 0; z < resolutionZ; ++z) {
		for (int x = 0; x < resolutionX; ++x) {
			bool walkable = m_data[x + z * resolutionX] == 1;

			if (!walkable) {
				gce::GameObject& cube = gce::GameObject::Create(*scene);
				cube.SetName("NavGridBlockedCube");

				float worldX = -sizeX / 2.0f + cellSize / 2.0f + x * cellSize;
				float worldZ = -sizeZ / 2.0f + cellSize / 2.0f + z * cellSize;

				cube.transform.SetLocalPosition({ worldX, -0.5f, worldZ });
				cube.transform.SetLocalScale({ cellSize, 0.1f, cellSize });

				auto& renderer = *cube.AddComponent<gce::MeshRenderer>();
				renderer.SetGeometry(gce::SHAPES.CUBE);
			}
		}
	}

	gce::GameObject& cubeBL = gce::GameObject::Create(*scene);
	cubeBL.SetName("NavGridDebugCube");
	cubeBL.transform.SetLocalPosition({ -sizeX / 2.0f + cellSize / 2.0f, -0.5f, -sizeZ / 2.0f + cellSize / 2.0f });
	cubeBL.transform.SetLocalScale({ cellSize, 0.1f, cellSize });
	auto& rendererBL = *cubeBL.AddComponent<gce::MeshRenderer>();
	rendererBL.SetGeometry(gce::SHAPES.CUBE);

	gce::GameObject& cubeCenter = gce::GameObject::Create(*scene);
	cubeCenter.SetName("NavGridDebugCubeCenter");
	cubeCenter.transform.SetLocalPosition({ 0.0f, -0.5f, 0.0f });
	cubeCenter.transform.SetLocalScale({ cellSize, 0.1f, cellSize });
	auto& rendererCenter = *cubeCenter.AddComponent<gce::MeshRenderer>();
	rendererCenter.SetGeometry(gce::SHAPES.CUBE);

	gce::GameObject& cubeTR = gce::GameObject::Create(*scene);
	cubeTR.SetName("NavGridDebugCubeTopRight");
	cubeTR.transform.SetLocalPosition({ sizeX / 2.0f - cellSize / 2.0f, -0.5f, sizeZ / 2.0f - cellSize / 2.0f });
	cubeTR.transform.SetLocalScale({ cellSize, 0.1f, cellSize });
	auto& rendererTR = *cubeTR.AddComponent<gce::MeshRenderer>();
	rendererTR.SetGeometry(gce::SHAPES.CUBE);
}

int NavGrid::ToIndex(const GridPosition& gridPos) {
	return gridPos.z * resolutionX + gridPos.x;
}
GridPosition NavGrid::ToGridPosition(const gce::Vector3f32& worldPos) {
	// Taking into account the gameobject position (origin is on the center of the grid)
	GridPosition gridPos;
	gce::Vector3f32 localPos = worldPos - m_pOwner->transform.GetWorldPosition();
	gridPos.x = static_cast<int>((localPos.x + sizeX / 2.0f) / cellSize);
	gridPos.z = static_cast<int>((localPos.z + sizeZ / 2.0f) / cellSize);

	// Clamp to grid size
	if (gridPos.x < 0) gridPos.x = 0;
	if (gridPos.x >= resolutionX) gridPos.x = resolutionX - 1;
	if (gridPos.z < 0) gridPos.z = 0;
	if (gridPos.z >= resolutionZ) gridPos.z = resolutionZ - 1;

	return gridPos;
}
gce::Vector3f32 NavGrid::ToWorldPosition(const GridPosition& gridPos) {
	// Taking into account the gameobject position (origin is on the center of the grid)
	gce::Vector3f32 localPos;
	localPos.x = gridPos.x * cellSize + cellSize / 2.0f - sizeX / 2.0f;
	localPos.z = gridPos.z * cellSize + cellSize / 2.0f - sizeZ / 2.0f;
	localPos.y = 0.0f;
	return localPos + m_pOwner->transform.GetWorldPosition();
}

float manhattan(const gce::Vector2i32& a, const gce::Vector2i32& b) {
	return static_cast<float>(abs(a.x - b.x) + abs(a.y - b.y));
}

std::vector<GridPosition> NavGrid::ComputePath(GridPosition _start, GridPosition _end) {
	const int width = resolutionX;
	const int height = resolutionZ;

	std::vector<GridPosition> path;

	std::vector<gce::Vector2i32> g_visited_points;
	std::vector<int> g_scores;

	char* buffer = m_data;

	gce::Vector2i32 goal_position = { _end.x, _end.z };
	gce::Vector2i32 player_position = { _start.x, _start.z };

	std::vector<GridCell> grid;
	grid.reserve(width * height);
	grid.resize(width * height);

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int index = y * width + x;
			grid[index].position = { x, y };
			grid[index].index = index;
			grid[index].visited = false;
			grid[index].dist_to_player = std::numeric_limits<int>::max();
			grid[index].dist_to_goal = manhattan(grid[index].position, goal_position);
			grid[index].from = nullptr;
			g_scores.push_back(0);
		}
	}

	std::priority_queue<GridCell*, std::vector<GridCell*>, GridCellComparator> queue;

	GridCell* start = &grid[ToIndex({ player_position.x, player_position.y })];
	start->dist_to_player = 0;
	start->dist_to_goal = manhattan(start->position, goal_position);

	queue.push(start);

	while (!queue.empty()) {
		GridCell* current_point = queue.top();
		queue.pop();

		if (current_point->visited)
			continue;

		g_visited_points.push_back(current_point->position);

		current_point->visited = true;

		if (current_point->position == goal_position) {
			std::vector<gce::Vector2i32> path_points;
			GridCell* point = current_point;
			while (point != nullptr) {
				path_points.push_back(point->position);
				point = point->from;
			}

			std::reverse(path_points.begin(), path_points.end());

			for (const gce::Vector2i32& point : path_points)
				path.push_back({ point.x, point.y });

			return path;
		}

		gce::Vector2i32 directions[4] = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} };

		for (const gce::Vector2i32& direction : directions) {
			gce::Vector2i32 neighbor_position = current_point->position + direction;

			// Security check to ensure neighbor is inside the grid
			if (neighbor_position.x < 0 || neighbor_position.x >= width ||
				neighbor_position.y < 0 || neighbor_position.y >= height)
				continue;

			int neighbor_index = ToIndex({ neighbor_position.x, neighbor_position.y });

			// skip non-walkable cells
			if (m_data[neighbor_index] == 0)
				continue;

			GridCell* neighbor = &grid[neighbor_index];

			if (neighbor->visited)
				continue;

			int new_neighbor_dist_to_player = current_point->dist_to_player + 1;

			if (new_neighbor_dist_to_player < neighbor->dist_to_player) {
				neighbor->from = current_point;
				neighbor->dist_to_player = new_neighbor_dist_to_player;
				neighbor->dist_to_goal = manhattan(neighbor->position, goal_position);

				queue.push(neighbor);
				g_scores[neighbor_index] = neighbor->dist_to_goal + neighbor->dist_to_player;
				g_visited_points.push_back(neighbor->position);
			}
		}
	}
	return path;
}