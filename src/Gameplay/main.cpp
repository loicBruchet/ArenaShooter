#include <windows.h>
#include <Engine.h>

#include "Framework/Utils/Json.hpp"
#include "Framework/PrefabRegistry.h"

#include "Framework/ResourceManager.h"

#include "Game/Levels.h"

int32 APIENTRY WinMain(HINSTANCE, HINSTANCE, PSTR, int32) \
{
	gce::Console::Init();
	gce::Console::Log("Starting GCE Engine...");

	gce::Console::Log(">> Creating GameManager...");
	gce::GameManager::Create();
	gce::Console::Log("Successfully created GameManager.", gce::Console::LogType::SUCCESS);

	gce::Console::Log(">> Creating ResourceManager...");
	RESOURCE_MANAGER;
	gce::Console::Log("Successfully created ResourceManager.", gce::Console::LogType::SUCCESS);

	gce::Console::Log(">> Creating Scene Object...");
	gce::Scene& scene = gce::Scene::Create();
	gce::Console::Log("Successfully created Scene Object.", gce::Console::LogType::SUCCESS);

	MainMenuLevel(scene);

	gce::WindowParam params;
	params.title = L"GCE Engine Window";
	params.width = 1280;
	params.height = 720;
	params.isFullScreen = true;
	params.isSplitScreen = false;
	params.screenDisposition = gce::SplitScreenDisposition::SQUARE_4_PLAYERS;

	std::cout << std::endl;
	std::cout << "===================================" << std::endl;
	std::cout << std::endl;

	gce::Console::Log("Engine initialized successfully!", gce::Console::LogType::SUCCESS);
	gce::Console::Log(">>Starting Game Loop...");
	gce::GameManager::Run(params);
	gce::Console::Log("Game Loop has ended.", gce::Console::LogType::SUCCESS);

	gce::Console::Log("Executing engine cleanup...");
	gce::GameManager::Destroy();
	gce::Console::Log("Engine cleanup complete. Exiting now.", gce::Console::LogType::SUCCESS);
	return 0;
}