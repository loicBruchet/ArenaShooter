#include "ShieldPrefab.h"

#include <Engine.h>
#include <Shapes.h>


ShieldPrefab::ShieldPrefab(gce::Scene& _scene, json* _properties) : Prefab(_scene, _properties)
{
	gce::BitMapBrush* bitmapBrushShield = new gce::BitMapBrush("res/ArenaShooter/UI/Shield_dark.png");
	gce::GameObject& shieldObject = m_root;
	shieldObject.SetName("ShieldBar");
	shieldObject.transform.SetLocalPosition({ 100.0f,200.0f,0.0f });
	shieldObject.transform.SetLocalScale({ 60.0f,60.0f,0.0f });
	gce::UiImage& shieldImage = *shieldObject.AddComponent<gce::UiImage>();
	shieldImage.btmBrush = bitmapBrushShield;
	shieldImage.InitializeImage({ 100.0f,100.0f }, { 0.0f,0.0f }, 1.0f);
}	
ShieldPrefab::~ShieldPrefab() {}