#include "pch.h"
#include "GameManager.h"
#include "Audio.h"

#include <ranges>

#include <Console.h>
#include <Tweens.hpp>

#include <RenderCamera.h>
#include <RenderWindow.h>
#include <RenderContext.h>
#include <LightManager.h>
#include <D12PipelineObject.h>

#include "Shapes.h"

#include "GameObject.h"
#include "Scene.h"
#include "Systems.h"
#include <InputsMethods.h>

namespace gce {
	Event<> GameManager::OnStart = Event<>();
	Event<> GameManager::OnUpdate = Event<>();

    GameManager::GameManager()
    {
        // Console Init
        Console::Init();

        // Render Init
        RenderContext::Init();
        RenderContext::InitUI();
        RenderContext::SetMSAA(MSAA_COUNT_X4);

        // GLOBAL STRUCTS Init
        SHAPES.Init();
        SHADERS.Init();

        defaultPso = new D12PipelineObject(SHADERS.VERTEX, SHADERS.PIXEL, SHADERS.HULL, SHADERS.DOMAIN_, SHADERS.ROOT_SIGNATURE);
		unlitPso = new D12PipelineObject(SHADERS.VERTEX, SHADERS.PIXEL_TEXTURE, SHADERS.HULL, SHADERS.DOMAIN_, SHADERS.ROOT_SIGNATURE_TEXTURE);
    }

    GameManager::~GameManager()
    {
        // GLOBAL STRUCTS UnInit
        SHADERS.UnInit();
        SHAPES.UnInit();

        // Console UnInit
        Console::UnInit();

        delete defaultPso;
    }

    D12PipelineObject const* GameManager::GetDefaultPso()
    {
        return s_pInstance->defaultPso;
    }

    D12PipelineObject const* GameManager::GetUnlitPso()
    {
		return s_pInstance->unlitPso;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // @brief Starts the game loop with the specified window parameters.
    // @param param The parameters for the game window, including title, dimensions, and screen disposition.
    // @note This function is called to begin the game loop, which handles rendering, input, and updates.
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void GameManager::GameLoop(WindowParam& param)
    {
        m_windowParam = param;

        if (param.isSplitScreen) m_pWindow = new SplitScreenWindow(param.title, param.width, param.height, param.screenDisposition);
        else m_pWindow = new RenderWindow(param.title, param.width, param.height, (param.isFullScreen ? FullScreenMode::BORDERLESS : FullScreenMode::WINDOWED));

        LightManager::SetLightsProperties(8.0f, 100.0f, 2.0f, 32.0f, 0.1f);

        m_lifespanSystem.HandleCreations();
        m_lifespanSystem.HandleDeletions();
		OnStart.Invoke();

        while (m_pWindow->IsOpen())
        {
            m_deltaTime = m_chrono.Reset();
            HandleFPS();

            InputSystem::HandleInputs();

            m_timeSinceFixedUpdate += m_deltaTime;
            while ( m_timeSinceFixedUpdate >= m_fixedDeltaTime )
            {
                m_scriptSystem.m_fixedUpdateEvent.Invoke();

                m_physicSystem.HandlePhysicComponents( m_fixedDeltaTime );

                for (Scene* const pScene : m_scenes) // TODO REMOVE
                    pScene->UpdateMatrix(); // TODO REMOVE

                m_physicSystem.HandlePhysicCollision3D();
                m_physicSystem.HandlePhysicCollisionResolve();

                for ( Scene* const pScene : m_scenes ) // TODO REMOVE
                    pScene->UpdateMatrix(); // TODO REMOVE

                m_physicSystem.HandlePhysicCollision2D();
                m_physicSystem.HandlePhysicCollisionResolve2D();

                for ( Scene* const pScene : m_scenes ) // TODO REMOVE
                    pScene->UpdateMatrix(); // TODO REMOVE

                m_timeSinceFixedUpdate -= m_fixedDeltaTime;
            }

            {
                // compute mouse delta robustly relative to the window center
                Window* window = GetWindow();
                Vector2i32 centerPos(window->GetWidth() / 2, window->GetHeight() / 2);

                // Read the current mouse position (client coords) once
                Vector2i32 currentMousePos = GetMousePosition();

                // Delta = movement since last frame (positive X = right, positive Y = down)

                if (m_cursorLocked) {
                    m_mouseDelta = { (float32)(currentMousePos.x - centerPos.x), (float32)(currentMousePos.y - centerPos.y) };
                }
                else {
                    m_mouseDelta = { (float32)(currentMousePos.x - m_lastMousePos.x), (float32)(currentMousePos.y - m_lastMousePos.y) };
                }

                // Immediately warp cursor back to center for the next frame so we never hit edges
                if (m_cursorLocked) {
                    SetMousePosition(centerPos);
                }

				m_lastMousePos = GetMousePosition();
            }

            m_scriptSystem.m_updateEvent.Invoke();
			OnUpdate.Invoke();

            m_statesSystem.HandleStateMachines();

            TweenSystem::Update( m_deltaTime );
            
            for ( Scene* const pScene : m_scenes )
                pScene->UpdateMatrix();

            m_uiSystem.HandleUiUpdate();

            m_renderSystem.Handle3DRendering();
            m_uiSystem.HandleUiRendering();

            m_pWindow->Display();

            m_lifespanSystem.HandleCreations();
            m_lifespanSystem.HandleDeletions();

            m_pWindow->Update();
            
            AudioUse::Update(m_deltaTime);
        }

        delete m_pWindow;
    }


    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    // @brief Updates the FPS counter and window title with the current FPS every second.
    // @note This function is called once per frame to calculate and display the FPS in the window title.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    void GameManager::HandleFPS()
    {
        if (m_deltaTime != 0.0f)
        {
            m_frameCount++;
            m_timeElapsed += m_deltaTime;

            if (m_timeElapsed >= 1.0f)
            {
                m_fps = static_cast<float>(m_frameCount) / m_timeElapsed;

                m_timeElapsed = 0.0f;
                m_frameCount = 0;
            }
        }
    }


}
