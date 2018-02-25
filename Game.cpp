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
	Sprite* testSprite = new Sprite(IMAGE_DISPLAYED);
	testSprite->ResizeSprite(RESOLUTION_X, RESOLUTION_Y);
	CreateInstance(new Entity(0, 0, 0, testSprite, 0));
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

	//Draw the Screen
	gfx.Render(entities);
}

void Game::GameShutdown()
{
	//Shutdown Graphics
	gfx.GraphicsShutdown();

	//Destroy Entities
	//DestroyInstance(...);
}

void Game::CreateInstance(Entity* entity)
{
	entities.push_back(entity);
}
