#pragma once

#include <Engine.h>
#include "Framework/Utils/Json.hpp"

gce::Quaternion ConvertBlenderQuatToEngine(const json& arr);
gce::Vector3f32 BlenderToEngineVec3(const json& arr);
void ApplyTransform(gce::GameObject& go, const json& t);

void DeserializeSceneFromJson(const json& stream, gce::Scene& scene);