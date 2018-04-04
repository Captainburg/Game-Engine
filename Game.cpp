#include "Game.h"

Game::Game()
	: selected(0)
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

	Model* droneModel = new Model("EvilDrone.x", gfx.getDevice());
	CreateInstance(new Drone(5, 0, -10, droneModel, 0));

	Model* globeModel = new Model("sphere.x", gfx.getDevice());
	CreateInstance(new Globe(-5, 0, -10, globeModel, 0));
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
		if (selected == (*it))
		{
			(*it)->BehaviorLoop();
		}
	}

	//Move Camera
	if (!selected)
	{
		gfx.Camera_Behavior();
	}

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

void Game::GetRay(int x, int y)
{
	selected = 0;
	float closest = FLT_MAX;

	for (auto it = entities.begin(); it != entities.end(); ++it) {
		if ((*it)->IsDrawable3D())
		{
			if (gfx.GetRay(x, y, *(*it)->GetModel()->GetBSphere()))
			{
				float dist = gfx.DistanceToCamera((*it)->GetX(), (*it)->GetY(), (*it)->GetZ());
				if (dist < closest)
				{
					selected = (*it);
					closest = dist;
				}
			}
		}
	}
}
