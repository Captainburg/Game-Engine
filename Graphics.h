#pragma once
#include "Font.h"
#include "GameClasses.h"
#include <vector>
#include "GameMacros.h"
#include "psystem.h"
#include <cstdlib>
#include <ctime>

class Graphics {
public:
	Graphics();
	virtual ~Graphics();

	float DistanceToCamera(float x, float y, float z);

	void InitLights();

	void Camera_Behavior();
	void Lights_Behavior();

	/** Graphics Initalization
	*
	* Initializes DirectX9 Graphics.
	*
	* @param hwnd The Handle to the window.
	* @return null
	*/
	void GraphicsInit(HWND hwnd);
	bool InitParticles();
	/** Graphics Initalization
	*
	* Shuts down the DirectX9 Graphics.
	*
	* @return null
	*/
	void GraphicsShutdown();
	/** Render
	*
	* Completes all rendering during the GameLoop().
	*
	* @param entities The entities to be drawn on screen.
	* @return null
	*/
	void Render(std::vector<Entity*> entities);
	/** getDevice
	*
	* Gets the device context.
	*
	* @return the device context.
	*/
	LPDIRECT3DDEVICE9 getDevice();

	bool GetRay(int x, int y, BoundingSphere BSphere);

	Ray CalcPickingRay(int x, int y);

	void TransformRay(Ray * ray, D3DXMATRIX * T);

	bool RaySphereIntTest(Ray * ray, BoundingSphere * sphere);

	int getFPS();

private:
	/** Initialize DirectX9 3D Device
	*
	* Initializes the DirectX9 3D Device.
	*
	* @param hWndTarget The Handle to the window.
	* @param Width The width of the window.
	* @param Height The height of the window.
	* @param bWindowed A boolean that's true if the game is windowed.
	* @param FullScreenFormat The fullscreen format.
	* @param pD3D The D3D pointer.
	* @param ppDevice The device to draw on.
	* @return null
	*/
	void InitDirect3DDevice(HWND hWndTarget, int Width, int Height, BOOL bWindowed, D3DFORMAT FullScreenFormat, LPDIRECT3D9 pD3D, LPDIRECT3DDEVICE9 * ppDevice);
	/** Load Sprite to Surface
	*
	* Loads a Sprite onto the surface.
	*
	* @param spr The Sprite of the image.
	* @param ppSurface The surface pointer's address.
	* @param pDevice A pointer to the device.
	* @return int fail or success condition.
	*/
	int LoadBitmapToSurface(Sprite * spr, LPDIRECT3DSURFACE9 * ppSurface, LPDIRECT3DDEVICE9 pDevice);
	/** Draw Image
	*
	* Draws an image on screen.
	*
	* @param x The x co-ordinate to draw the image at.
	* @param y The y co-ordinate to draw the image at.
	* @param Pitch The width of the drawing surface.
	* @param pData The drawing surface.
	* @param img The image to be drawn.
	* @return null
	*/
	void DrawImage(int x, int y, int Pitch, DWORD * pData, BITMAP * img);
	/** Draw Pixel
	*
	* Draws a pixel on screen.
	*
	* @param x The x co-ordinate to draw the pixel at.
	* @param y The y co-ordinate to draw the pixel at.
	* @param Pitch The width of the drawing surface.
	* @param pData The drawing surface.
	* @param color The color of the pixel to be drawn.
	* @return null
	*/
	void DrawPixel(int x, int y, int Pitch, DWORD* pData, DWORD color);

	LPDIRECT3D9 g_pD3D;
	LPDIRECT3DDEVICE9 g_pDevice;
	HWND g_hWndMain;
	Font* font;

	//Camera Position
	bool bSelected;
	double cam_x;
	double cam_y;
	double cam_z;

	double cam_spin;
	double cam_angle;

	//Benchmarking
	int FPS;
	int frameCount;
	DWORD lastFrame;
	PSystem* Sno;
};