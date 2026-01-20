#include "GameManager.h"

#ifndef ENGINE_GAME_MANAGER_INL_INCLUDED
#define ENGINE_GAME_MANAGER_INL_INCLUDED

#include <InputsMethods.h>
#include "Scene.h"

namespace gce {
	inline void GameManager::Create() { if (s_pInstance != nullptr) return; s_pInstance = new GameManager(); }
	inline void GameManager::Run(WindowParam& param) { if (s_pInstance == nullptr) return; s_pInstance->GameLoop(param); }
	inline void GameManager::Destroy() { delete s_pInstance; }

	inline float32 GameManager::DeltaTime() { return s_pInstance->m_deltaTime; }
	inline float32 GameManager::FixedDeltaTime() { return s_pInstance->m_fixedDeltaTime; }
	inline float32 GameManager::FPS() { return s_pInstance->m_fps; }
	inline Window* GameManager::GetWindow() { return s_pInstance->m_pWindow; }
	inline WindowParam& GameManager::GetWindowParam() { return s_pInstance->m_windowParam; }
	inline RenderSystem& GameManager::GetRenderSystem() { return s_pInstance->m_renderSystem; }
	inline LifespanSystem& GameManager::GetLifespanSystem() { return s_pInstance->m_lifespanSystem; }
	inline ScriptSystem& GameManager::GetScriptSystem() { return s_pInstance->m_scriptSystem; }
	inline PhysicSystem& GameManager::GetPhysicSystem() { return s_pInstance->m_physicSystem; }
	inline StatesSystem& GameManager::GetStatesSystem() { return s_pInstance->m_statesSystem; }
	inline UiSystem& GameManager::GetUiSystem() { return s_pInstance->m_uiSystem; }
	inline std::vector<Camera*>& GameManager::GetMainCameras() { return s_pInstance->m_mainCameras; }

	inline std::vector<Scene*>& GameManager::GetScenes()
	{
		return s_pInstance->m_scenes;
	}

	inline Scene* GameManager::GetActiveScene() {
		if (s_pInstance->m_scenes.size() > 0)
		{
			return s_pInstance->m_scenes[0];
		}
		return nullptr;
	}

	inline void GameManager::LockCursor() {
		s_pInstance->m_lastMousePosBeforeLock = GetMousePosition();
		HideMouseCursor();
		LockMouseCursor();
		Window* window = GetWindow();
		Vector2i32 centerPos(window->GetWidth() / 2, window->GetHeight() / 2);
		SetMousePosition(centerPos);
		s_pInstance->m_cursorLocked = true;
	}

	inline void GameManager::UnlockCursor() {
		SetMousePosition(s_pInstance->m_lastMousePosBeforeLock);
		ShowMouseCursor();
		UnlockMouseCursor();
		s_pInstance->m_cursorLocked = false;
	}

	inline bool GameManager::IsCursorLocked() {
		return s_pInstance->m_cursorLocked;
	}

	inline gce::Vector2f32 GameManager::GetMouseDelta()
	{
		return s_pInstance->m_mouseDelta;
	}
	inline gce::GameObject* GameManager::GetObjectByName(std::string_view name)
	{
		Scene* activeScene = GetActiveScene();
		if (activeScene)
		{
			return (activeScene->GetObjectByName(name));
		}
		return nullptr;
	}
	inline void GameManager::ChangeScene(Scene* newScene)
	{
		gce::GameManager::UnlockCursor();
		if (newScene == nullptr)
			return;

		OnStart.ClearListeners();
		OnUpdate.ClearListeners();

		// destroy everything related to the current scene
		Scene* currentScene = GetActiveScene();
		if (currentScene)
		{
			delete currentScene;
		}

		// clear current scenes
		s_pInstance->m_scenes.clear();
		// Add the new scene
		s_pInstance->m_scenes.push_back(newScene);
		gce::GameManager::UnlockCursor();
	}
}

#endif