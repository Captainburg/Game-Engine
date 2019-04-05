#pragma once
#include <d3dx9.h>

class Sprite {
public:
	Sprite(char filename[]);
	virtual ~Sprite();

	/** Resize Sprite
	*
	* Forces this sprite to a specified size.
	*
	* @param X The width of the sprite to be forced to.
	* @param Y The height of the sprite to be forced to.
	* @return null
	*/
	void ResizeSprite(int X, int Y);
	/** GetBitmap
	*
	* Returns this Sprites bitmap data.
	*
	* @return BITMAP* A pointer to this Sprites bitmap data.
	*/
	BITMAP* GetBitmap();
	/** GetSurface
	*
	* Returns this Sprites surface data.
	*
	* @return BITMAP* A pointer to this Sprites surface data.
	*/
	LPDIRECT3DSURFACE9 GetSurface();
	/** Get Filename Pointer
	*
	* Get a Filename pointer.
	*
	* @return char* a Filename pointer.
	*/
	char* GetFileName();
	/** Give Surface Pointer
	*
	* Give this sprite a surface pointer.
	*
	* @return null
	*/
	void GiveSurfacePointer(LPDIRECT3DSURFACE9 newSurf);
	/** Is Surface Loaded
	*
	* Returns true if surface is loaded.
	*
	* @return bool true if surface is loaded.
	*/
	bool IsSurfaceLoaded();

private:
	/** Flip Sprite
	*
	* Forces this sprite to be flipped.
	*
	* @return null
	*/
	void FlipSprite();

	BITMAP* bitmap;
	char* fname;
	bool bSurfaceLoaded;
	LPDIRECT3DSURFACE9 pSurface;
};