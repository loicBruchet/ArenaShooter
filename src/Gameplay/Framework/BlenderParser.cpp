#include "BlenderParser.h"

#include <windows.h>
#include <Engine.h>
#include <Shapes.h>
#include "define.h"

#include "Framework/Scripts/NavGrid.h"
#include "Framework/Scripts/FreeCamera.h"

#include "Game/Scripts/Player/PlayerCamera.h"
#include "Game/Scripts/Player/PlayerMovements.h"
#include "Game/Scripts/Weapon/Weapon.h"

#include "Framework/Utils/Json.hpp"
#include "Framework/PrefabRegistry.h"

#include "Framework/ResourceManager.h"

#include "Framework/Prefab.h"

#include <cmath>

#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

gce::Quaternion ConvertBlenderQuatToEngine(const json& arr) {
	float b[4] = { 0,0,0,1 };
	if (arr.is_array() && arr.size() >= 4)
		for (int i = 0; i < 4; i++) b[i] = arr[i].get<float>();

	auto Normalize = [](float q[4])
		{
			float n = sqrtf(q[0] * q[0] + q[1] * q[1] + q[2] * q[2] + q[3] * q[3]);
			if (n > 0.0f)
				for (int i = 0; i < 4; i++) q[i] /= n;
		};

	auto Mul = [](const float a[4], const float b[4], float o[4])
		{
			o[3] = a[3] * b[3] - a[0] * b[0] - a[1] * b[1] - a[2] * b[2];
			o[0] = a[3] * b[0] + a[0] * b[3] + a[1] * b[2] - a[2] * b[1];
			o[1] = a[3] * b[1] - a[0] * b[2] + a[1] * b[3] + a[2] * b[0];
			o[2] = a[3] * b[2] + a[0] * b[1] - a[1] * b[0] + a[2] * b[3];
		};

	constexpr float inv_sqrt2 = 0.70710678f;

	float r[4] = { -inv_sqrt2,0,0,inv_sqrt2 }; // -90 X
	float r_inv[4] = { inv_sqrt2,0,0,inv_sqrt2 };

	float best[4]; float bestScore = FLT_MAX;

	for (int m = 0; m < 2; m++)
	{
		float q[4] = {
			m == 0 ? b[0] : b[1],
			m == 0 ? b[1] : b[2],
			m == 0 ? b[2] : b[3],
			m == 0 ? b[3] : b[0]
		};

		float tmp[4], out[4];
		Mul(r, q, tmp);
		Mul(tmp, r_inv, out);
		Normalize(out);

		float n = fabsf(1.f - (out[0] * out[0] + out[1] * out[1]
			+ out[2] * out[2] + out[3] * out[3]));

		if (n < bestScore)
		{
			bestScore = n;
			memcpy(best, out, sizeof(best));
		}
	}

	return { best[0],best[1],best[2],best[3] };
}

gce::Vector3f32 BlenderToEngineVec3(const json& arr) {
	if (!arr.is_array() || arr.size() < 3)
		return { 0,0,0 };

	return {
		arr[0].get<float>(),
		arr[2].get<float>(),
	   -arr[1].get<float>()
	};
}

void ApplyTransform(gce::GameObject& go, const json& t) {
	go.transform.SetWorldPosition(BlenderToEngineVec3(t["position"]));
	go.transform.SetWorldRotation(ConvertBlenderQuatToEngine(t["rotation"]));
	go.transform.SetWorldScale({
		t["scale"][0].get<float>(),
		t["scale"][2].get<float>(),
		t["scale"][1].get<float>()
	});
}

void DeserializeSceneFromJson(const json& stream, gce::Scene& scene)
{
	gce::Texture* texture = RESOURCE_MANAGER.GetTexture("TexturesTest");

	for (const auto& obj : stream["entities"])
	{
		const std::string name = obj["name"];
		const std::string type = obj["entity_type"];

		// ===== PREFAB =====
		if (name.find("Prefab") != std::string::npos)
		{
			// try to get custom properties
			json* properties = nullptr;
			if (obj.contains("properties"))
				properties = new json(obj["properties"]);

			std::string prefabName = name;
			auto pos = name.find('_');
			if (pos != std::string::npos) {
				prefabName = name.substr(0, pos);
			}

			Prefab* prefab = PREFAB_REGISTRY.CreatePrefab(prefabName, scene, properties);
			gce::GameObject& prefabGO = prefab->GetRootObject();
			ApplyTransform(prefabGO, obj["transform"]);
			continue;
		}

		gce::GameObject& go = gce::GameObject::Create(scene);

		std::string* pName = new std::string(name);

		go.SetName(pName->c_str());

		// ========= EMPTY ===========
		if (type == "EMPTY")
		{
			auto& bc = *go.AddComponent<gce::BoxCollider>();
			bc.m_localOffSet = { 0,0,0 };
			bc.localScale = { 2,2,2 };

			ApplyTransform(go, obj["transform"]);
			continue;
		}

		// ========= MESH ===========
		if (type != "MESH" || !obj.contains("mesh"))
			continue;

		// ===== PATHFINDING GRID =====
		if (name.find("Pathfinding") != std::string::npos)
		{
			int minX = INT_MAX, maxX = INT_MIN;
			int minZ = INT_MAX, maxZ = INT_MIN;

			for (const auto& v : obj["mesh"]["vertices"])
			{
				gce::Vector3f32 p = BlenderToEngineVec3(v);
				minX = std::min(minX, (int)p.x);
				maxX = std::max(maxX, (int)p.x);
				minZ = std::min(minZ, (int)p.z);
				maxZ = std::max(maxZ, (int)p.z);
			}

			gce::Vector3f32 vert0 = BlenderToEngineVec3(obj["mesh"]["vertices"][0]);
			gce::Vector3f32 vert1 = BlenderToEngineVec3(obj["mesh"]["vertices"][1]);

			float cellSize = std::abs(vert1.x - vert0.x);

			int resolutionX = (maxX - minX) / cellSize;
			int resolutionZ = (maxZ - minZ) / cellSize;

			float sizeX = resolutionX * cellSize;
			float sizeZ = resolutionZ * cellSize;

			go.SetName("NavGridObjet");

			ApplyTransform(go, obj["transform"]);

			NavGrid& navGrid = *go.AddScript<NavGrid>();
			navGrid.resolutionX = resolutionX;
			navGrid.resolutionZ = resolutionZ;
			navGrid.cellSize = cellSize;

			navGrid.sizeX = sizeX;
			navGrid.sizeZ = sizeZ;

			// Allocate buffer
			char* data = new char[resolutionX * resolutionZ];
			memset(data, 0, resolutionX * resolutionZ); // Default 0 (non-walkable)

			// Fill with vertices
			for (const auto& v : obj["mesh"]["vertices"])
			{
				gce::Vector3f32 p = BlenderToEngineVec3(v);

				int cellX = static_cast<int>((p.x - minX) / cellSize);
				int cellZ = static_cast<int>((p.z - minZ) / cellSize);

				// Clamp to grid
				if (cellX < 0 || cellX >= resolutionX) continue;
				if (cellZ < 0 || cellZ >= resolutionZ) continue;

				data[cellX + cellZ * resolutionX] = 1;
			}

			navGrid.Init(data);

			continue;
		}

		// ===== GEOMETRY =====
		if (!obj["mesh"].contains("vertices") || !obj["mesh"].contains("indices"))
			continue;

		auto& mr = *go.AddComponent<gce::MeshRenderer>();

		const auto& jsonVerts = obj["mesh"]["vertices"];
		const auto& jsonUVs = obj["mesh"].value("uvs", json::array());
		const auto& jsonNormals = obj["mesh"].value("normals", json::array());

		std::vector<gce::Vertex> verts;
		verts.reserve(jsonVerts.size());

		for (size_t i = 0; i < jsonVerts.size(); ++i)
		{
			gce::Vertex v{};
			v.pos = BlenderToEngineVec3(jsonVerts[i]);

			if (i < jsonUVs.size())
			{
				v.uv = {
					jsonUVs[i][0],
					1.0f - jsonUVs[i][1].get<float>()
				};
			}

			if (i < jsonNormals.size())
			{
				auto n = jsonNormals[i];
				v.normal = gce::Vector3f32(n[0].get<float>(), n[2].get<float>(), -n[1].get<float>());
			}

			verts.push_back(v);
		}

		std::vector<uint32_t> indices;
		for (auto& i : obj["mesh"]["indices"])
			indices.push_back(i);

		gce::Geometry* geo = new gce::Geometry(
			verts.data(), verts.size(),
			indices.data(), indices.size()
		);

		mr.SetGeometry(geo);
		mr.SetAlbedoTexture(texture);

		if (name.find("Collider") != std::string::npos)
			go.AddComponent<gce::BoxCollider>();

		ApplyTransform(go, obj["transform"]);
	}
}