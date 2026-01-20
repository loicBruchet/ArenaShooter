#include "MenuStartPrefab.h"
#include <Engine.h>
#include "Game/Scripts/Player/PlayerMovements.h"
MenuStartPrefab::MenuStartPrefab(gce::Scene& _scene, json* _properties): Prefab(_scene, _properties)
{
	gce::BitMapBrush* bitmapbruchBackground = new gce::BitMapBrush("res/Exemple/place-holder.png");

	gce::BitMapBrush* bitmapbruchButtonStart = new gce::BitMapBrush("res/Exemple/place-holder.png");

	gce::BitMapBrush* bitmapbruchButtonQuit = new gce::BitMapBrush("res/Exemple/place-holder.png");

	gce::GameObject& uiButtonStart = gce::GameObject::Create(_scene);
	uiButtonStart.SetName("UI_ButtonStart");
	uiButtonStart.transform.SetLocalPosition({ 640.0f, 400.0f, 0.0f });
	uiButtonStart.transform.SetLocalScale({ 100.0f,80.0f,0.0f });
	gce::UiButton& uiButtonImageStart = *uiButtonStart.AddComponent<gce::UiButton>();
	uiButtonImageStart.pBitMapBrush = bitmapbruchButtonStart;
	uiButtonImageStart.pHoverBitMapBrush = bitmapbruchButtonStart;
	uiButtonImageStart.m_onClickEvent.AddListener(&MenuStartPrefab::OnClickStart, this);

	gce::GameObject& uiButtonQuit = gce::GameObject::Create(_scene);
	uiButtonQuit.SetName("UI_ButtonQuit");
	uiButtonQuit.transform.SetLocalPosition({ 640.0f, 600.0f, 0.0f });
	uiButtonQuit.transform.SetLocalScale({ 100.0f,80.0f,0.0f });
	gce::UiButton& uiButtonImageQuit = *uiButtonQuit.AddComponent<gce::UiButton>();
	uiButtonImageQuit.pBitMapBrush = bitmapbruchButtonQuit;
	uiButtonImageQuit.pHoverBitMapBrush = bitmapbruchButtonQuit;
	uiButtonImageQuit.m_onClickEvent.AddListener(&MenuStartPrefab::OnClickQuit, this);

	gce::GameObject& uiObject = gce::GameObject::Create(_scene);
	uiObject.SetName("UI_Background");
	uiObject.transform.SetLocalPosition({ 640.0f, 400.0f, 0.0f });
	uiObject.transform.SetLocalScale({ 1280.0f,800.0f,0.0f });
	gce::UiImage& uiImage = *uiObject.AddComponent<gce::UiImage>();
	uiImage.btmBrush = bitmapbruchBackground;
	uiImage.InitializeImage({ 100.0f,100.0f }, { 0.0f,0.0f }, 0.0f);
}

MenuStartPrefab::~MenuStartPrefab(){}

void MenuStartPrefab::OnClickStart()
{
	gce::GameObject* player = gce::GameManager::GetActiveScene()->GetObjectByName("Player");
	player->SetActive(true);
	player->GetScript<PlayerMovements>()->Start();
	gce::GameManager::GetActiveScene()->GetObjectByName("UI_ButtonStart")->SetActive(false);
	gce::GameManager::GetActiveScene()->GetObjectByName("UI_Background")->SetActive(false);
	gce::GameManager::GetActiveScene()->GetObjectByName("UI_ButtonQuit")->SetActive(false);
	gce::GameManager::LockCursor();
}
void MenuStartPrefab::OnClickQuit()
{
	gce::GameManager::Destroy();
	exit(0);
}
