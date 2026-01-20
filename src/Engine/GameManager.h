#ifndef ENGINE_GAME_MANAGER_H_INCLUDED
#define ENGINE_GAME_MANAGER_H_INCLUDED

#include <SplitScreenWindow.h>
#include "define.h"

#include "Chrono.h"
#include "Systems.h"
#include "Components/Camera.h"

namespace gce {

struct WindowParam
{
    WStringView title = L"Game Window"; // Do not work because of GameManager::HandleFPS
	uint16 width = 1280;
	uint16 height = 720;
	bool isFullScreen = false;
    bool isSplitScreen = false;
	SplitScreenDisposition screenDisposition = SQUARE_4_PLAYERS;
    
    WindowParam() = default;
};

class Scene;
class Window;

class GameManager final
{
public:
    static void Create();
    static void Run(WindowParam& param);
    static void Destroy();

    [[nodiscard]] static float32 DeltaTime();
    [[nodiscard]] static float32 FixedDeltaTime();
    [[nodiscard]] static float32 FPS();
	[[nodiscard]] static Window* GetWindow();
	[[nodiscard]] static WindowParam& GetWindowParam();
    [[nodiscard]] static RenderSystem& GetRenderSystem();
    [[nodiscard]] static LifespanSystem& GetLifespanSystem();
    [[nodiscard]] static ScriptSystem& GetScriptSystem();
	[[nodiscard]] static PhysicSystem& GetPhysicSystem();
	[[nodiscard]] static StatesSystem& GetStatesSystem();
	[[nodiscard]] static UiSystem& GetUiSystem();
    [[nodiscard]] static std::vector<Camera*>& GetMainCameras();
	[[nodiscard]] static std::vector<Scene*>& GetScenes();
	[[nodiscard]] static Scene* GetActiveScene();
	[[nodiscard]] static void LockCursor();
    [[nodiscard]] static void UnlockCursor();
	[[nodiscard]] static bool IsCursorLocked();
    [[nodiscard]] static gce::Vector2f32 GetMouseDelta();
	[[nodiscard]] static gce::GameObject* GetObjectByName(std::string_view name);

	[[nodiscard]] static void ChangeScene(Scene* newScene);

	static D12PipelineObject const* GetDefaultPso();
	static D12PipelineObject const* GetUnlitPso();

	static Event<> OnStart;
	static Event<> OnUpdate;

    GameManager();
    ~GameManager();

    void GameLoop(WindowParam& param);

private:
    inline static GameManager* s_pInstance = nullptr;

	bool m_cursorLocked = false;

	gce::Vector2i32 m_lastMousePosBeforeLock = { 0, 0 };

	gce::Vector2f32 m_lastMousePos = { 0.0f, 0.0f };

	gce::Vector2f32 m_mouseDelta = { 0.0f, 0.0f };

    void HandleFPS();

    Window* m_pWindow;
    WindowParam m_windowParam;

    float m_timeElapsed;
    uint16 m_frameCount;

    Chrono m_chrono;
    float32 m_deltaTime;
    float32 m_fixedDeltaTime = 1.0f / 120.0f;
    float32 m_timeSinceFixedUpdate;
    float32 m_fps;

    RenderSystem m_renderSystem;
    LifespanSystem m_lifespanSystem;
    PhysicSystem m_physicSystem;
    ScriptSystem m_scriptSystem;
    UiSystem m_uiSystem;
	StatesSystem m_statesSystem;
    
    std::vector<Scene*> m_scenes;
    std::vector<Camera*> m_mainCameras;

	D12PipelineObject const* defaultPso;
	D12PipelineObject const* unlitPso;

    friend class Scene;
};
}

#include "GameManager.inl"

#endif