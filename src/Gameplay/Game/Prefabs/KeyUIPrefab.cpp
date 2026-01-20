#include "KeyUIPrefab.h"

#include <Engine.h>
#include <Shapes.h>

KeyUIPrefab::KeyUIPrefab(gce::Scene& _scene, json* _properties) : Prefab(_scene, _properties) {

	gce::BitMapBrush* Texture = new gce::BitMapBrush("res/ArenaShooter/UI/Key_Icon.png");

	gce::GameObject& uiObject = m_root;
	uiObject.SetName("UI_Key");
	uiObject.transform.SetLocalPosition({ 90.0f, 250.0f, 0.0f });
	uiObject.transform.SetLocalScale({ 70.0f,70.0f,0.0f });
	gce::UiImage& uiImage = *uiObject.AddComponent<gce::UiImage>();
	uiImage.btmBrush = Texture;
	uiImage.InitializeImage({ 100.0f,100.0f }, { 0.0f,0.0f }, 1.0f);
	

}
KeyUIPrefab::~KeyUIPrefab() {}