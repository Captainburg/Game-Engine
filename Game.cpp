#include "Game.h"

Game::Game()
{
}

Game::~Game()
{
}

void Game::GameInit(HWND hwnd)
{
	//Initialize Graphics
	gfx.GraphicsInit(hwnd);

	//Initialize Entities
	Model* tigerModel = new Model("tiger2.x", gfx.getDevice());
	CreateInstance(new Tiger(0, 0, 0, tigerModel, 0));

	Model* planeModel = new Model("airplane2.x", gfx.getDevice());
	CreateInstance(new Plane(0, 0, -25, planeModel, 0));
}

void Game::GameLoop()
{
	//Temporary GameLoop
	if (GetAsyncKeyState(VK_ESCAPE))
	{
		PostQuitMessage(0);
	}

	//Each Entity Follows it's Behavior
	for (auto it = entities.begin(); it != entities.end(); ++it) {
		(*it)->BehaviorLoop();
	}

	//Move Camera
	gfx.Camera_Behavior();

	//Move Lights
	gfx.Lights_Behavior();

	//Draw the Screen
	gfx.Render(entities);
}

void Game::GameShutdown()
{
	//Destroy Entities
	for (auto it = entities.begin(); it != entities.end(); ++it) {
		delete (*it);
	}

	//Shutdown Graphics
	gfx.GraphicsShutdown();
}

void Game::CreateInstance(Entity* entity)
{
	entities.push_back(entity);
}
