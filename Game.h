#pragma once
#include "Rendering/Graphics.h"

class Game {
public:
	Game();
	virtual ~Game();

	/** Game Initalization
	*
	* Initializes the Game State.
	*
	* @param hwnd The Handle to the window.
	* @return null
	*/
	void GameInit(HWND hwnd);
	/** Game Loop
	*
	* Loops through all the objects so they can all behave and be drawn.
	*
	* @return null
	*/
	void GameLoop();
	/** Game Shutdown
	*
	* Shuts down the Game State.
	*
	* @return null
	*/
	void GameShutdown();

	/** Create Instance
	*
	* Creates a new instance of an Entity and adds it to the entity list for looping.
	*
	* @param entity A pointer to a new entity.
	* @return null
	*
	**Entity Creation Example:**
	Sprite* testSprite = new Sprite(IMAGE_NAME);
	CreateInstance(new Entity(0, 0, 0, testSprite, 0));
	*/
	inline void CreateInstance(Entity* entity);
	//void DestroyInstance(...);

	inline void CreateMirror(Mirror* mirror);
	//void DestroyInstance(...);

	void GetRay(int x, int y);

private:
	Graphics gfx;
	Entity* selected;
	std::vector<Entity*> entities;
	std::vector<Mirror*> mirrors;
};