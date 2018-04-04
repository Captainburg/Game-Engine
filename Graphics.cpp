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
	, cam_x(0)
	, cam_y(4)
	, cam_z(5)
	, cam_spin(0)
	, cam_angle(0.64)
	, bSelected(false)
	, Sno(0)
{
}

Graphics::~Graphics()
{
}

float Graphics::DistanceToCamera(float x, float y, float z)
{
	float x_diff = x - cam_x;
	float y_diff = y - cam_y;
	float z_diff = z - cam_z;

	return sqrt(sqrt(x_diff * x_diff + y_diff * y_diff) * sqrt(x_diff * x_diff + y_diff * y_diff) + z_diff * z_diff);
}

// this is the function that sets up the lights and materials
void Graphics::InitLights()
{
	D3DLIGHT9 light;    // create the light struct

	//POINT
	ZeroMemory(&light, sizeof(light));    // clear out the light struct for use
	light.Type = D3DLIGHT_POINT;    // make the light type 'directional light'
	light.Diffuse = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);    // set the light's color
	light.Range = 30;
	light.Position = D3DXVECTOR3(0, 2, -25);

	g_pDevice->SetLight(0, &light);    // send the light struct properties to light #0
	g_pDevice->LightEnable(0, FALSE);    // turn on light #0

	//DIRECTIONAL
	ZeroMemory(&light, sizeof(light));    // clear out the light struct for use
	light.Type = D3DLIGHT_DIRECTIONAL;    // make the light type 'directional light'
	light.Diffuse = D3DXCOLOR(1, 0, 0, 1);    // set the light's color
	light.Direction = D3DXVECTOR3(1, 0, 0);

	g_pDevice->SetLight(1, &light);    // send the light struct properties to light #0
	g_pDevice->LightEnable(1, FALSE);    // turn on light #0

	//SPOT
	ZeroMemory(&light, sizeof(light));    // clear out the light struct for use
	light.Type = D3DLIGHT_SPOT;    // make the light type 'directional light'
	light.Diffuse = D3DXCOLOR(0, 0, 1, 1);    // set the light's color
	light.Direction = D3DXVECTOR3(0, 0, -1);
	light.Range = 30;
	light.Position = D3DXVECTOR3(0, 0, 5);
	light.Phi = D3DXToRadian(40);
	light.Theta = D3DXToRadian(20);

	g_pDevice->SetLight(2, &light);    // send the light struct properties to light #0
	g_pDevice->LightEnable(2, FALSE);    // turn on light #0
}


void Graphics::Lights_Behavior()
{
	//Ambient ON
	if (GetAsyncKeyState(0x30))
	{
		g_pDevice->SetRenderState(D3DRS_AMBIENT, 0xffffffff);
	}
	//Ambient OFF
	if (GetAsyncKeyState(0x50))
	{
		g_pDevice->SetRenderState(D3DRS_AMBIENT, 0);
	}
	//Point ON
	if (GetAsyncKeyState(0x39))
	{
		g_pDevice->LightEnable(0, TRUE);
	}
	//Point OFF
	if (GetAsyncKeyState(0x4f))
	{
		g_pDevice->LightEnable(0, FALSE);
	}
	//Directional ON
	if (GetAsyncKeyState(0x38))
	{
		g_pDevice->LightEnable(1, TRUE);
	}
	//Directional OFF
	if (GetAsyncKeyState(0x49))
	{
		g_pDevice->LightEnable(1, FALSE);
	}
	//Spot ON
	if (GetAsyncKeyState(0x37))
	{
		g_pDevice->LightEnable(2, TRUE);
	}
	//Spot OFF
	if (GetAsyncKeyState(0x55))
	{
		g_pDevice->LightEnable(2, FALSE);
	}
}

void Graphics::Camera_Behavior()
{
	//W Forward
	if (GetAsyncKeyState(0x57))
	{
		cam_z -= 0.05;
	}
	//A Left
	if (GetAsyncKeyState(0x41))
	{
		cam_x += 0.05;
	}
	//D Right
	if (GetAsyncKeyState(0x44))
	{
		cam_x -= 0.05;
	}
	//S Backward
	if (GetAsyncKeyState(0x53))
	{
		cam_z += 0.05;
	}
	//Shift Downward
	if (GetAsyncKeyState(VK_SHIFT))
	{
		cam_y -= 0.05;
	}
	//Space Upward
	if (GetAsyncKeyState(VK_SPACE))
	{
		cam_y += 0.05;
	}
	//CCW - Spin Left
	if (GetAsyncKeyState(VK_NUMPAD4))
	{
		cam_spin += 0.005;
	}
	//CW - Spin Right
	if (GetAsyncKeyState(VK_NUMPAD6))
	{
		cam_spin -= 0.005;
	}
	//Angle Up
	if (GetAsyncKeyState(VK_NUMPAD8))
	{
		cam_angle -= 0.005;
	}
	//Angle Down
	if (GetAsyncKeyState(VK_NUMPAD2))
	{
		cam_angle += 0.005;
	}

	if (cam_angle < 0.001) {
		cam_angle = 0.001;
	}
	if (cam_angle > 0.999) {
		cam_angle = 0.999;
	}
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
	(*ppDevice)->SetRenderState(D3DRS_ZENABLE, TRUE);

	//Turn on Lighting
	(*ppDevice)->SetRenderState(D3DRS_LIGHTING, TRUE);

	// Turn on ambient lighting 
	//(*ppDevice)->SetRenderState(D3DRS_AMBIENT, 0xffffffff);
}

void Graphics::GraphicsInit(HWND hwnd)
{
	g_hWndMain = hwnd;
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	InitDirect3DDevice(g_hWndMain, RESOLUTION_X, RESOLUTION_Y, IS_WINDOWED, D3DFMT_X8R8G8B8, g_pD3D, &g_pDevice);
	font = new Font(g_pDevice);
	font->LoadAlphabet("Alphabet vSmall.bmp", 8, 16);
	InitLights();
	InitParticles();
}


bool Graphics::InitParticles()
{
	// seed random number generator
	srand((unsigned int)time(0));

	//
	// Create Snow System.
	//

	BoundingBox* boundingBox = new BoundingBox;
	boundingBox->_min = D3DXVECTOR3(-40.0f, -40.0f, -40.0f);
	boundingBox->_max = D3DXVECTOR3(40.0f, 40.0f, 40.0f);
	Sno = new Snow(boundingBox, 5000);
	Sno->init(g_pDevice, "snowflake.dds");

	//
	// Set projection matrix.
	//
	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
		&proj,
		D3DX_PI / 4.0f, // 45 - degree
		(float)RESOLUTION_X / (float)RESOLUTION_Y,
		1.0f,
		5000.0f);
	g_pDevice->SetTransform(D3DTS_PROJECTION, &proj);

	return true;
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
	Delete<PSystem*>(Sno);
}

void Graphics::Render(std::vector<Entity*> entities)
{
	//Declare LockedRect and the Back Surface
	D3DLOCKED_RECT LockedRect;
	LPDIRECT3DSURFACE9 pBackSurf = 0;

	Sno->update(1.0f / FPS);

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
			RECT rect = { 0, 0, static_cast<long>(d3dsd.Width), static_cast<long>(d3dsd.Height) };

			//Select the Bitmap on the DestSurface
			RECT rectDest = { static_cast<long>((*it)->GetX()), static_cast<long>((*it)->GetY()), static_cast<long>((*it)->GetX() + d3dsd.Width), static_cast<long>((*it)->GetY() + d3dsd.Height) };

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
			(*it)->SetupMatrices(g_pDevice, cam_x, cam_y, cam_z, cam_spin, cam_angle);

			// Meshes are divided into subsets, one for each material. Render them in a loop
			for (DWORD i = 0; i < (*it)->GetModel()->NumMaterials(); i++)
			{
				// Set the material and texture for this subset
				g_pDevice->SetMaterial(&((*it)->GetModel()->GetMaterial())[i]);
				g_pDevice->SetTexture(0, (*it)->GetModel()->GetTexture()[i]);

				// Draw the mesh subset
				(*it)->GetModel()->GetMesh()->DrawSubset(i);

				// Render the bounding sphere with alpha blending so we can see through it.
				/*g_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
				g_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				g_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

				D3DMATERIAL9 blue = BLUE_MTRL;
				blue.Diffuse.a = 0.25f; // 25% opacity
				g_pDevice->SetMaterial(&blue);
				(*it)->GetModel()->GetSphere()->DrawSubset(0);

				g_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);*/
			}

		}
	}
	D3DXMATRIXA16 snowWorld;
	D3DXMatrixTranslation(&snowWorld, 0, 0, 0);
	g_pDevice->SetTransform(D3DTS_WORLD, &snowWorld);
	Sno->render();

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

LPDIRECT3DDEVICE9 Graphics::getDevice()
{
	return g_pDevice;
}

bool Graphics::GetRay(int x, int y, BoundingSphere BSphere)
{
	Ray ray = CalcPickingRay(x, y);

	// transform the ray to world space
	D3DXMATRIX view;
	g_pDevice->GetTransform(D3DTS_VIEW, &view);

	D3DXMATRIX viewInverse;
	D3DXMatrixInverse(&viewInverse, 0, &view);

	TransformRay(&ray, &viewInverse);

	// test for a hit
	return RaySphereIntTest(&ray, &BSphere);
}

Ray Graphics::CalcPickingRay(int x, int y)
{
	float px = 0.0f;
	float py = 0.0f;

	D3DVIEWPORT9 vp;
	g_pDevice->GetViewport(&vp);

	D3DXMATRIX proj;
	g_pDevice->GetTransform(D3DTS_PROJECTION, &proj);

	px = (((2.0f*x) / vp.Width) - 1.0f) / proj(0, 0);
	py = (((-2.0f*y) / vp.Height) + 1.0f) / proj(1, 1);

	Ray ray;
	ray._origin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	ray._direction = D3DXVECTOR3(px, py, 1.0f);

	return ray;
}

void Graphics::TransformRay(Ray* ray, D3DXMATRIX* T)
{
	// transform the ray's origin, w = 1.
	D3DXVec3TransformCoord(
		&ray->_origin,
		&ray->_origin,
		T);

	// transform the ray's direction, w = 0.
	D3DXVec3TransformNormal(
		&ray->_direction,
		&ray->_direction,
		T);

	// normalize the direction
	D3DXVec3Normalize(&ray->_direction, &ray->_direction);
}

bool Graphics::RaySphereIntTest(Ray* ray, BoundingSphere* sphere)
{
	D3DXVECTOR3 v = ray->_origin - sphere->_center;

	float b = 2.0f * D3DXVec3Dot(&ray->_direction, &v);
	float c = D3DXVec3Dot(&v, &v) - (sphere->_radius * sphere->_radius);

	// find the discriminant
	float discriminant = (b * b) - (4.0f * c);

	// test for imaginary number
	if (discriminant < 0.0f)
		return false;

	discriminant = sqrtf(discriminant);

	float s0 = (-b + discriminant) / 2.0f;
	float s1 = (-b - discriminant) / 2.0f;

	// if a solution is >= 0, then we intersected the sphere
	if (s0 >= 0.0f || s1 >= 0.0f)
		return true;

	return false;
}

int Graphics::getFPS()
{
	return FPS;
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