#include <iostream>
#include <string>
#include "Graphics.h"

Graphics::Graphics()
	: g_pD3D(0)
	, g_pDevice(0)
	, g_hWndMain(0)
	, FPS(60)
	, frameCount(0)
	, lastFrame(GetTickCount())
	, font(0)
{
}

Graphics::~Graphics()
{
}

void Graphics::InitDirect3DDevice(HWND hWndTarget, int Width, int Height, BOOL bWindowed, D3DFORMAT FullScreenFormat, LPDIRECT3D9 pD3D, LPDIRECT3DDEVICE9* ppDevice) {
	
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;

	if (*ppDevice)
	{
		(*ppDevice)->Release();
	}

	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));
	pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);

	d3dpp.BackBufferWidth = Width;
	d3dpp.BackBufferHeight = Height;
	d3dpp.BackBufferFormat = bWindowed ? d3ddm.Format : FullScreenFormat;
	d3dpp.BackBufferCount = 1;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hWndTarget;
	d3dpp.Windowed = bWindowed;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.FullScreen_RefreshRateInHz = 0;//default refresh rate
	d3dpp.PresentationInterval = bWindowed ? 0 : D3DPRESENT_INTERVAL_IMMEDIATE;
	d3dpp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;

	pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWndTarget, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, ppDevice);

	// Turn on the zbuffer
	(*ppDevice)->SetRenderState(D3DRS_ZENABLE, TRUE); // NEW

	// Turn on ambient lighting 
	(*ppDevice)->SetRenderState(D3DRS_AMBIENT, 0xffffffff); // NEW
}

void Graphics::GraphicsInit(HWND hwnd)
{
	g_hWndMain = hwnd;
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	InitDirect3DDevice(g_hWndMain, RESOLUTION_X, RESOLUTION_Y, IS_WINDOWED, D3DFMT_X8R8G8B8, g_pD3D, &g_pDevice);
	font = new Font(g_pDevice);
	font->LoadAlphabet("Alphabet vSmall.bmp", 8, 16);
}

void Graphics::GraphicsShutdown()
{
	if (g_pDevice)
	{
		g_pDevice->Release();
	}

	if (g_pD3D)
	{
		g_pD3D->Release();
	}

	if (font)
	{
		font->UnloadAlphabet();
	}
}

void Graphics::Render(std::vector<Entity*> entities)
{
	//Declare LockedRect and the Back Surface
	D3DLOCKED_RECT LockedRect;
	LPDIRECT3DSURFACE9 pBackSurf = 0;

	//FPS Translate to String
	std::string s = std::to_string(FPS);
	char const *pFPS = s.c_str();

	//Clear Device
	g_pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);

	//pBackSurface = the back buffer
	g_pDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackSurf);

	//Declare Surface Pointer
	LPDIRECT3DSURFACE9 pSurface = 0;

					//---DRAW BITMAP BLOCK---//

	for (std::vector<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it) {
		if ((*it)->IsDrawable2D())
		{
			//Load The Sprite to the Surface
			LoadBitmapToSurface((*it)->GetSprite(), &pSurface, g_pDevice);

			//Get the Surface Info
			D3DSURFACE_DESC d3dsd;
			pSurface->GetDesc(&d3dsd);//get info about surface

			//Select the Bitmap on the Surface
			RECT rect = {0, 0, static_cast<long>(d3dsd.Width), static_cast<long>(d3dsd.Height)};

			//Select the Bitmap on the DestSurface
			RECT rectDest = { (*it)->GetX(), (*it)->GetY(), (*it)->GetX() + static_cast<long>(d3dsd.Width), (*it)->GetY() + static_cast<long>(d3dsd.Height) };

			//Copy it to the Back Surface
			D3DXLoadSurfaceFromSurface(pBackSurf, NULL, &rectDest, pSurface, NULL, &rect, D3DX_FILTER_TRIANGLE, 0);

			//Zero Out Pointer
			pSurface = 0;
		}
	}
					//---DRAW MODEL BLOCK---//

	g_pDevice->BeginScene();
	for (std::vector<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it) {
		if ((*it)->IsDrawable3D())
		{
			// Setup the world, view, and projection matrices
			SetupMatrices();

			// Meshes are divided into subsets, one for each material. Render them in a loop
			for (DWORD i = 0; i < (*it)->GetModel()->NumMaterials(); i++)
			{
				// Set the material and texture for this subset
				g_pDevice->SetMaterial(&((*it)->GetModel()->GetMaterial())[i]);
				g_pDevice->SetTexture(0, (*it)->GetModel()->GetTexture()[i]);

				// Draw the mesh subset
				(*it)->GetModel()->GetMesh()->DrawSubset(i);
			}

		}
	}
	g_pDevice->EndScene();

					//---DRAW TEXT BLOCK---//
	
	//Lock the back surface
	pBackSurf->LockRect(&LockedRect, NULL, 0);

	//Pointer to the locked bits
	DWORD* pData = (DWORD*)(LockedRect.pBits);

	//Draw FPS
	font->DrawString(RESOLUTION_X - s.size() * 8, 0, pFPS, TRUE, D3DCOLOR_ARGB(255, 255, 0, 255), (DWORD*)LockedRect.pBits, LockedRect.Pitch);

	//Unlock back Surface
	pBackSurf->UnlockRect();

	//Release Back Surface
	pBackSurf->Release();

	//Clear Pointers
	pBackSurf = 0;
	pData = 0;
	
					//---SWAP BUFFERS---//
	g_pDevice->Present(NULL, NULL, NULL, NULL);

	//Frame Count
	if (lastFrame < GetTickCount() - 1000)
	{
		FPS = frameCount;
		frameCount = 0;
		lastFrame = GetTickCount();
	}
	frameCount++;
}

void Graphics::SetupMatrices() {

	// For our world matrix, we will just leave it as the identity
	D3DXMATRIXA16 matWorld;
	D3DXMatrixRotationY(&matWorld, timeGetTime() / 1000.0f); //This Rotates the Tiger
	g_pDevice->SetTransform(D3DTS_WORLD, &matWorld);

	// Set up our view matrix. A view matrix can be defined given an eye point,
	// a point to lookat, and a direction for which way is up. Here, we set the
	// eye five units back along the z-axis and up three units, look at the 
	// origin, and define "up" to be in the y-direction.
	D3DXVECTOR3 vEyePt(0.0f, 3.0f, -5.0f);
	D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);
	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);
	g_pDevice->SetTransform(D3DTS_VIEW, &matView);

	// For the projection matrix, we set up a perspective transform (which
	// transforms geometry from 3D view space to 2D viewport space, with
	// a perspective divide making objects smaller in the distance). To build
	// a perpsective transform, we need the field of view (1/4 pi is common),
	// the aspect ratio, and the near and far clipping planes (which define at
	// what distances geometry should be no longer be rendered).
	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4, 1.0f, 1.0f, 100.0f);
	g_pDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}

LPDIRECT3DDEVICE9 Graphics::getDevice()
{
	return g_pDevice;
}

int Graphics::LoadBitmapToSurface(Sprite* spr, LPDIRECT3DSURFACE9* ppSurface, LPDIRECT3DDEVICE9 pDevice) {


	//load bitmap onto surface
	if (!spr->IsSurfaceLoaded())
	{
		//create surface for bitmap
		LPDIRECT3DSURFACE9 tempSurf;
		pDevice->CreateOffscreenPlainSurface(spr->GetBitmap()->bmWidth, spr->GetBitmap()->bmHeight, D3DFMT_X8R8G8B8, D3DPOOL_SCRATCH, &tempSurf, NULL);
		
		//Load Bitmap to Surface
		RECT rect = { 0, 0, spr->GetBitmap()->bmWidth, spr->GetBitmap()->bmHeight };
		D3DXLoadSurfaceFromMemory(tempSurf, NULL, NULL, spr->GetBitmap()->bmBits, D3DFMT_R8G8B8, spr->GetBitmap()->bmWidthBytes, NULL, &rect, D3DX_FILTER_NONE, 0);

		//Surface Pointer Handoff
		spr->GiveSurfacePointer(tempSurf);
	}

	//Sprite has already loaded Surface
	*ppSurface = spr->GetSurface();

	return S_OK;
}

void Graphics::DrawImage(int x, int y, int Pitch, DWORD* pData, BITMAP* img)
{
	int width = img->bmWidth;
	int bOffset = img->bmBitsPixel / 8;

	byte* pBytes = (byte*)img->bmBits;
	for (int iy = img->bmHeight - 1; iy >= 0; iy--) {
		for (int ix = 0; ix < width; ix++) {
			pBytes += bOffset;
			DrawPixel(ix + x, iy + y, Pitch, pData, D3DCOLOR_XRGB(pBytes[0], pBytes[1], pBytes[2]));
		}
	}
}

void Graphics::DrawPixel(int x, int y, int Pitch, DWORD* pData, DWORD color)
{
		//Y Value, Find Width, X Value
	pData[y * Pitch / 4 + x] = color;
}