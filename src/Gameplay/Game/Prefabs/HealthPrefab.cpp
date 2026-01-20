#include "HealthPrefab.h"

#include <Engine.h>
#include <Shapes.h>

#include "Framework/ResourceManager.h"

HealthPrefab::HealthPrefab(gce::Scene& _scene, json* _properties) : Prefab(_scene, _properties)
{
	gce::BitMapBrush* bitmapBrushHealth = new gce::BitMapBrush("res/ArenaShooter/UI/2PV.png");

	gce::GameObject& healthObject = m_root;
	healthObject.SetName("HealthBar");
	healthObject.transform.SetLocalPosition({ 100.0f,100.0f,0.0f });
	healthObject.transform.SetLocalScale({ 60.0f,60.0f,0.0f });
	gce::UiImage& healthImage = *healthObject.AddComponent<gce::UiImage>();
	healthImage.btmBrush = bitmapBrushHealth;
	healthImage.InitializeImage({ 100.0f,100.0f }, { 0.0f,0.0f }, 1.0f);
}
HealthPrefab::~HealthPrefab() {}