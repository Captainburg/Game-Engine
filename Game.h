#pragma once
#include "Graphics.h"

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
	*/
	inline void CreateInstance(Entity* entity);
	//void DestroyInstance(...);

private:
	Graphics gfx;
	std::vector<Entity*> entities;
};