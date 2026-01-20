#pragma once

#include <Engine.h>

void MainMenuLevel(gce::Scene& scene);
void GameLevel(gce::Scene& scene);
void GameOver(gce::Scene& scene);
void WinLevel(gce::Scene& scene);

//class LevelManager
//{
//public:
//	gce::Scene* currentScene = nullptr;
//	gce::Scene* m_mainMenuScene = nullptr;
//	gce::Scene* m_gameOverScene = nullptr;
//private:
//	void LoadMainMenu(gce::Scene& scene);
//	void LoadGameLevel(gce::Scene& scene);
//	void LoadGameOver(gce::Scene& scene);
//
//};