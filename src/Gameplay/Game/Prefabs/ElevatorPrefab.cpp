#include "ElevatorPrefab.h"

#include <Engine.h>
#include <Shapes.h>

#include "Framework/ResourceManager.h"
#include "Framework/PrefabRegistry.h"								
#include "Game/Scripts/Elevator.h"
#include "Game/Tags.h"
ElevatorPrefab::ElevatorPrefab(gce::Scene& scene, json* _properties) : Prefab(scene, _properties)
{
	gce::Texture* BRICK_texture = RESOURCE_MANAGER.GetTexture("TexturesTest");
	gce::GameObject& elevator_object = m_root;
	elevator_object.SetName("Elevator");
	elevator_object.SetTags(TAG_DEFAULT | TAG_ENTITY );
	elevator_object.transform.SetWorldPosition({ -25.0f, 10.0f, -420.0f });
	elevator_object.transform.SetLocalScale({ 5.0f,2.0f,5.0f });

	gce::MeshRenderer& elevator_mesh = *elevator_object.AddComponent<gce::MeshRenderer>();
	elevator_mesh.SetGeometry(gce::SHAPES.CUBE);
	elevator_mesh.SetAlbedoTexture(BRICK_texture);

	gce::BoxCollider& elevator_collider = *elevator_object.AddComponent<gce::BoxCollider>();
	elevator_collider.m_localOffSet = { 0.0f, 0.0f, 0.0f };
	elevator_collider.localScale = { 1.0f, 1.0f, 1.0f };

	Elevator elevator = *elevator_object.AddScript<Elevator>();
}
ElevatorPrefab::~ElevatorPrefab() {}