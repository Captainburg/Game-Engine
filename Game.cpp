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
	Sprite* baboonSprite = new Sprite(IMAGE_DISPLAYED);
	baboonSprite->ResizeSprite(RESOLUTION_X, RESOLUTION_Y);
	CreateInstance(new Entity(0, 0, 0, baboonSprite, 0));

	Model* tigerModel = new Model(MODEL_DISPLAYED, gfx.getDevice());
	CreateInstance(new Entity(0, 0, 0, tigerModel, 0));
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
