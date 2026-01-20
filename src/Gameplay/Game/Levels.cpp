#include "Levels.h"

#include <Audio.h>

#include "Framework/PrefabRegistry.h"
#include "Framework/ResourceManager.h"

#include "Framework/BlenderParser.h"

void MainMenuLevel(gce::Scene& scene) {
	gce::GameObject& cameraObject = gce::GameObject::Create(scene);
	cameraObject.SetName("Camera");
	gce::Camera& camera = *cameraObject.AddComponent<gce::Camera>();
	cameraObject.transform.SetLocalPosition({ 0.0f, 0.0f, -10.0f });
	camera.perspective.nearPlane = 0.001f;
	camera.perspective.farPlane = 500.0f;
	camera.perspective.fov = gce::PI / 4;
	camera.SetType(gce::PERSPECTIVE);
	camera.SetMainCamera();

	gce::BitMapBrush* bitmapbruchBackground = new gce::BitMapBrush("res/ArenaShooter/UI/MainMenu/Fond_test.png");

	gce::BitMapBrush* bitmapbruchTitle = new gce::BitMapBrush("res/ArenaShooter/UI/MainMenu/EXTRA_TEMPORIS.png");

	gce::BitMapBrush* bitmapbruchButtonStart = new gce::BitMapBrush("res/ArenaShooter/UI/MainMenu/Start.png");

	gce::BitMapBrush* bitmapbruchButtonQuit = new gce::BitMapBrush("res/ArenaShooter/UI/MainMenu/quit.png");

	gce::GameObject& uiButtonStart = gce::GameObject::Create(scene);
	uiButtonStart.SetName("UI_ButtonStart");
	uiButtonStart.transform.SetLocalPosition({ 960.0f, 630.0f, 0.0f });
	uiButtonStart.transform.SetLocalScale({ 304,78.0f,0.0f });
	gce::UiButton& uiButtonImageStart = *uiButtonStart.AddComponent<gce::UiButton>();
	uiButtonImageStart.pBitMapBrush = bitmapbruchButtonStart;
	uiButtonImageStart.pHoverBitMapBrush = bitmapbruchButtonStart;

	uiButtonImageStart.m_onClickEvent.AddListener([]() {
		gce::Scene& newScene = gce::Scene::Create();
		gce::GameManager::ChangeScene(&newScene);
		GameLevel(newScene);
	});


	gce::GameObject& uiButtonQuit = gce::GameObject::Create(scene);
	uiButtonQuit.SetName("UI_ButtonQuit");
	uiButtonQuit.transform.SetLocalPosition({ 1750.0f, 970.0f, 0.0f });
	uiButtonQuit.transform.SetLocalScale({ 159.0f,83.0f,0.0f });
	gce::UiButton& uiButtonImageQuit = *uiButtonQuit.AddComponent<gce::UiButton>();
	uiButtonImageQuit.pBitMapBrush = bitmapbruchButtonQuit;
	uiButtonImageQuit.pHoverBitMapBrush = bitmapbruchButtonQuit;

	uiButtonImageQuit.m_onClickEvent.AddListener([]() {
		exit(0);
		});


	gce::GameObject& uiObject = gce::GameObject::Create(scene);
	uiObject.SetName("UI_Background");
	uiObject.transform.SetLocalPosition({ 960.0f, 540.0f, 0.0f });
	uiObject.transform.SetLocalScale({ 1920.0f,1080.0f,0.0f });
	gce::UiImage& uiImage = *uiObject.AddComponent<gce::UiImage>();
	uiImage.btmBrush = bitmapbruchBackground;
	uiImage.InitializeImage({ 100.0f,100.0f }, { 0.0f,0.0f }, 1.0f);

	gce::GameObject& uiTittle = gce::GameObject::Create(scene);
	uiTittle.SetName("UI_Background");
	uiTittle.transform.SetLocalPosition({ 960.0f, 425.0f, 0.0f });
	uiTittle.transform.SetLocalScale({ 1612.0f,222.0f,0.0f });
	gce::UiImage& uiImageTittle = *uiTittle.AddComponent<gce::UiImage>();
	uiImageTittle.btmBrush = bitmapbruchTitle;
	uiImageTittle.InitializeImage({ 100.0f,100.0f }, { 0.0f,0.0f }, 1.0f);

	gce::GameManager::UnlockCursor();
}

void GameLevel(gce::Scene& scene) {
	gce::Console::Log(">> Loading Scene data from Disk...");
	json jsonScene = JSONFILE::loadFromFile("../../res/SampleScene.json");
	DeserializeSceneFromJson(jsonScene, scene);
	gce::Console::Log("Successfully loaded Scene data from disk.", gce::Console::LogType::SUCCESS);

	//PREFAB_REGISTRY.CreatePrefab("PlayerPrefab", scene);
	//PREFAB_REGISTRY.CreatePrefab("ChromePrefab", scene);
	//PREFAB_REGISTRY.CreatePrefab("DixoPrefab", scene);
	//PREFAB_REGISTRY.CreatePrefab("SilverPrefab", scene);
	//PREFAB_REGISTRY.CreatePrefab("NexronPrefab", scene);
	//PREFAB_REGISTRY.CreatePrefab("MenuStartPrefab", scene);
	PREFAB_REGISTRY.CreatePrefab("ElevatorPrefab", scene);
	//PREFAB_REGISTRY.CreatePrefab("CollectablePrefab", scene);
	//PREFAB_REGISTRY.CreatePrefab("KeyPrefab", scene);
	//PREFAB_REGISTRY.CreatePrefab("BreakableObjectPrefab", scene);

	//PREFAB_REGISTRY.CreatePrefab("KeyUIPrefab", scene);

	gce::GameObject& directionalLightObject = gce::GameObject::Create(scene);
	directionalLightObject.SetName("DirectionalLight");
	gce::Light& directionalLight = *directionalLightObject.AddComponent<gce::Light>();
	directionalLight.DefaultDirectionLight();
	directionalLight.intensity = 2.0f;
	directionalLight.direction = gce::Vector3f32(0.0f, -1.0f, -0.5f).Normalize();

	gce::AudioUse::Play(RESOURCE_MANAGER.GetSound("BossMusic"), false);
	gce::AudioUse::SetVolumeOfSound("BossMusic", 50);
}

void GameOver(gce::Scene& scene)
{
	gce::GameObject& cameraObject = gce::GameObject::Create(scene);
	cameraObject.SetName("Camera");
	gce::Camera& camera = *cameraObject.AddComponent<gce::Camera>();
	cameraObject.transform.SetLocalPosition({ 0.0f, 0.0f, -10.0f });
	camera.perspective.nearPlane = 0.001f;
	camera.perspective.farPlane = 500.0f;
	camera.perspective.fov = gce::PI / 4;
	camera.SetType(gce::PERSPECTIVE);
	camera.SetMainCamera();

	gce::BitMapBrush* bitmapbruchBackground = new gce::BitMapBrush("res/ArenaShooter/UI/GameOver/fond_GO.png");

	gce::BitMapBrush* bitmapbruchGameOver = new gce::BitMapBrush("res/ArenaShooter/UI/GameOver/GAME_OVER.png");

	gce::BitMapBrush* bitmapbruchButtonRestart = new gce::BitMapBrush("res/ArenaShooter/UI/GameOver/Retry_GO.png");

	gce::BitMapBrush* bitmapbruchButtonBackToMenu = new gce::BitMapBrush("res/ArenaShooter/UI/GameOver/back2Menu_GO.png");
	
	gce::BitMapBrush* bitmapbruchBordure = new gce::BitMapBrush("res/ArenaShooter/UI/bordure.png");

	gce::GameObject& uiButtonStart = gce::GameObject::Create(scene);
	uiButtonStart.SetName("UI_ButtonStart");
	uiButtonStart.transform.SetLocalPosition({ 960.0f, 630.0f, 0.0f });
	uiButtonStart.transform.SetLocalScale({ 284.0f,60.0f,0.0f });
	gce::UiButton& uiButtonImageStart = *uiButtonStart.AddComponent<gce::UiButton>();
	uiButtonImageStart.pBitMapBrush = bitmapbruchButtonRestart;
	uiButtonImageStart.pHoverBitMapBrush = bitmapbruchButtonRestart;

	uiButtonImageStart.m_onClickEvent.AddListener([]() {
		gce::Scene& newScene = gce::Scene::Create();
		gce::GameManager::ChangeScene(&newScene);
		GameLevel(newScene);
		});


	gce::GameObject& uiButtonQuit = gce::GameObject::Create(scene);
	uiButtonQuit.SetName("UI_ButtonQuit");
	uiButtonQuit.transform.SetLocalPosition({ 960.0f, 730.0f, 0.0f });
	uiButtonQuit.transform.SetLocalScale({ 404.0f,84.0f,0.0f });
	gce::UiButton& uiButtonImageQuit = *uiButtonQuit.AddComponent<gce::UiButton>();
	uiButtonImageQuit.pBitMapBrush = bitmapbruchButtonBackToMenu;
	uiButtonImageQuit.pHoverBitMapBrush = bitmapbruchButtonBackToMenu;

	uiButtonImageQuit.m_onClickEvent.AddListener([]() {
		gce::Scene& newScene = gce::Scene::Create();
		gce::GameManager::ChangeScene(&newScene);
		MainMenuLevel(newScene);
		});


	gce::GameObject& uiObject = gce::GameObject::Create(scene);
	uiObject.SetName("UI_Background");
	uiObject.transform.SetLocalPosition({ 960.0f, 540.0f, 0.0f });
	uiObject.transform.SetLocalScale({ 1920.0f,1080.0f,0.0f });
	gce::UiImage& uiBackground = *uiObject.AddComponent<gce::UiImage>();
	uiBackground.btmBrush = bitmapbruchBackground;
	uiBackground.InitializeImage({ 100.0f,100.0f }, { 0.0f,0.0f }, 1.0f);

	gce::GameObject& uiTittle = gce::GameObject::Create(scene);
	uiTittle.SetName("UI_Background");
	uiTittle.transform.SetLocalPosition({ 960.0f, 425.0f, 0.0f });
	uiTittle.transform.SetLocalScale({ 1076.0f,132.0f,0.0f });
	gce::UiImage& uiImageTittle = *uiTittle.AddComponent<gce::UiImage>();
	uiImageTittle.btmBrush = bitmapbruchGameOver;
	uiImageTittle.InitializeImage({ 100.0f,100.0f }, { 0.0f,0.0f }, 1.0f);

	gce::GameObject& uiBordure = gce::GameObject::Create(scene);
	uiBordure.SetName("UI_Bordure");
	uiBordure.transform.SetLocalPosition({ 960.0f, 540.0f, 0.0f });
	uiBordure.transform.SetLocalScale({ 1920.0f,1080.0f,0.0f });
	gce::UiImage& uiImage = *uiBordure.AddComponent<gce::UiImage>();
	uiImage.btmBrush = bitmapbruchBordure;
	uiImage.InitializeImage({ 100.0f,100.0f }, { 0.0f,0.0f }, 1.0f);

	gce::GameManager::UnlockCursor();
}

void WinLevel(gce::Scene& scene)
{
	gce::GameObject& cameraObject = gce::GameObject::Create(scene);
	cameraObject.SetName("Camera");
	gce::Camera& camera = *cameraObject.AddComponent<gce::Camera>();
	cameraObject.transform.SetLocalPosition({ 0.0f, 0.0f, -10.0f });
	camera.perspective.nearPlane = 0.001f;
	camera.perspective.farPlane = 500.0f;
	camera.perspective.fov = gce::PI / 4;
	camera.SetType(gce::PERSPECTIVE);
	camera.SetMainCamera();

	gce::BitMapBrush* bitmapbruchBackground = new gce::BitMapBrush("res/ArenaShooter/UI/Win/fond_W.png");

	gce::BitMapBrush* bitmapbruchGameOver = new gce::BitMapBrush("res/ArenaShooter/UI/Win/Mission _Completed.png");

	gce::BitMapBrush* bitmapbruchButtonRestart = new gce::BitMapBrush("res/ArenaShooter/UI/Win/Retry_W.png");

	gce::BitMapBrush* bitmapbruchButtonBackToMenu = new gce::BitMapBrush("res/ArenaShooter/UI/Win/title_Menu_W.png");

	gce::BitMapBrush* bitmapbruchBordure = new gce::BitMapBrush("res/ArenaShooter/UI/Win/cadre_W.png");

	gce::GameObject& uiButtonStart = gce::GameObject::Create(scene);
	uiButtonStart.SetName("UI_ButtonStart");
	uiButtonStart.transform.SetLocalPosition({ 960.0f, 650.0f, 0.0f });
	uiButtonStart.transform.SetLocalScale({ 282.0f,58.0f,0.0f });
	gce::UiButton& uiButtonImageStart = *uiButtonStart.AddComponent<gce::UiButton>();
	uiButtonImageStart.pBitMapBrush = bitmapbruchButtonRestart;
	uiButtonImageStart.pHoverBitMapBrush = bitmapbruchButtonRestart;

	uiButtonImageStart.m_onClickEvent.AddListener([]() {
		gce::Scene& newScene = gce::Scene::Create();
		gce::GameManager::ChangeScene(&newScene);
		GameLevel(newScene);
		});


	gce::GameObject& uiButtonQuit = gce::GameObject::Create(scene);
	uiButtonQuit.SetName("UI_ButtonQuit");
	uiButtonQuit.transform.SetLocalPosition({ 960.0f, 760.0f, 0.0f });
	uiButtonQuit.transform.SetLocalScale({ 404.0f,84.0f,0.0f });
	gce::UiButton& uiButtonImageQuit = *uiButtonQuit.AddComponent<gce::UiButton>();
	uiButtonImageQuit.pBitMapBrush = bitmapbruchButtonBackToMenu;
	uiButtonImageQuit.pHoverBitMapBrush = bitmapbruchButtonBackToMenu;

	uiButtonImageQuit.m_onClickEvent.AddListener([]() {
		gce::Scene& newScene = gce::Scene::Create();
		gce::GameManager::ChangeScene(&newScene);
		MainMenuLevel(newScene);
		});


	gce::GameObject& uiObject = gce::GameObject::Create(scene);
	uiObject.SetName("UI_Background");
	uiObject.transform.SetLocalPosition({ 960.0f, 540.0f, 0.0f });
	uiObject.transform.SetLocalScale({ 1920.0f,1080.0f,0.0f });
	gce::UiImage& uiBackground = *uiObject.AddComponent<gce::UiImage>();
	uiBackground.btmBrush = bitmapbruchBackground;
	uiBackground.InitializeImage({ 100.0f,100.0f }, { 0.0f,0.0f }, 1.0f);

	gce::GameObject& uiTittle = gce::GameObject::Create(scene);
	uiTittle.SetName("UI_Title");
	uiTittle.transform.SetLocalPosition({ 960.0f, 425.0f, 0.0f });
	uiTittle.transform.SetLocalScale({ 1108.0f,321.0f,0.0f });
	gce::UiImage& uiImageTittle = *uiTittle.AddComponent<gce::UiImage>();
	uiImageTittle.btmBrush = bitmapbruchGameOver;
	uiImageTittle.InitializeImage({ 100.0f,100.0f }, { 0.0f,0.0f }, 1.0f);

	gce::GameObject& uiBordure = gce::GameObject::Create(scene);
	uiBordure.SetName("UI_Bordure");
	uiBordure.transform.SetLocalPosition({ 960.0f, 540.0f, 0.0f });
	uiBordure.transform.SetLocalScale({ 1920.0f,1080.0f,0.0f });
	gce::UiImage& uiImage = *uiBordure.AddComponent<gce::UiImage>();
	uiImage.btmBrush = bitmapbruchBordure;
	uiImage.InitializeImage({ 100.0f,100.0f }, { 0.0f,0.0f }, 1.0f);

	gce::GameManager::UnlockCursor();
}