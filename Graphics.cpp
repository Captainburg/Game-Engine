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
	, cam_y(7)
	, cam_z(15)
	, cam_spin(0)
	, cam_angle(0.64)
	, bSelected(false)
	, Sno(0)
	, mirrorVertices(0)
	, MirrorTex(0)
	, MirrorMtrl(WHITE_MTRL)
{
}

Graphics::~Graphics()
{
}

float Graphics::DistanceToCamera(float x, float y, float z)
{
	double x_diff = x - cam_x;
	double y_diff = y - cam_y;
	double z_diff = z - cam_z;

	return (float)sqrt(sqrt(x_diff * x_diff + y_diff * y_diff) * sqrt(x_diff * x_diff + y_diff * y_diff) + z_diff * z_diff);
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
	double yWeakness = 1 - pow(fabs(cam_angle - 0.5) * 2, 2);
	D3DXVECTOR3 vDir((FLOAT)sin(cam_spin * D3DX_PI * 2) * yWeakness, (FLOAT)cos(cam_angle * D3DX_PI), (FLOAT)sin((cam_spin - 0.25) * D3DX_PI * 2) * yWeakness);
	
	float x_diff = 0;
	float y_diff = 0;
	float z_diff = 0;

	//W Forward
	if (GetAsyncKeyState(0x57))
	{
		z_diff -= 0.05;
	}
	//A Left
	if (GetAsyncKeyState(0x41))
	{
		x_diff += 0.05;
	}
	//D Right
	if (GetAsyncKeyState(0x44))
	{
		x_diff -= 0.05;
	}
	//S Backward
	if (GetAsyncKeyState(0x53))
	{
		z_diff += 0.05;
	}
	//Shift Downward
	if (GetAsyncKeyState(VK_SHIFT))
	{
		y_diff -= 0.05;
	}
	//Space Upward
	if (GetAsyncKeyState(VK_SPACE))
	{
		y_diff += 0.05;
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

	cam_x += -z_diff * vDir.x + -x_diff * vDir.z;
	cam_y += -z_diff * vDir.y + y_diff;
	cam_z += -z_diff * vDir.z + x_diff * vDir.x;

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
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hWndTarget;
	d3dpp.Windowed = bWindowed;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.FullScreen_RefreshRateInHz = 0;//default refresh rate
	d3dpp.PresentationInterval = bWindowed ? 0 : D3DPRESENT_INTERVAL_IMMEDIATE;
	d3dpp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;

	pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWndTarget, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, ppDevice);

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
	InitMirrors();
}

void Graphics::InitMirrors()
{
	g_pDevice->CreateVertexBuffer(
		36 * sizeof(Vertex),
		0, // usage
		Vertex::FVF,
		D3DPOOL_MANAGED,
		&mirrorVertices,
		0);

	Vertex* v = 0;
	mirrorVertices->Lock(0, 0, (void**)&v, 0);

	// mirror (front)
	v[0] = Vertex(2.5f, -2.5f, 2.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[1] = Vertex(2.5f, 2.5f, 2.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[2] = Vertex(-2.5f, 2.5f, 2.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);

	v[3] = Vertex(2.5f, -2.5f, 2.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[4] = Vertex(-2.5f, 2.5f, 2.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[5] = Vertex(-2.5f, -2.5f, 2.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	// mirror (right)
	v[6] = Vertex(-2.5f, -2.5f, 2.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[7] = Vertex(-2.5f, 2.5f, 2.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[8] = Vertex(-2.5f, 2.5f, -2.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);

	v[9] = Vertex(-2.5f, -2.5f, 2.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[10] = Vertex(-2.5f, 2.5f, -2.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[11] = Vertex(-2.5f, -2.5f, -2.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	// mirror (left)
	v[12] = Vertex(2.5f, -2.5f, -2.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[13] = Vertex(2.5f, 2.5f, -2.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[14] = Vertex(2.5f, 2.5f, 2.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);

	v[15] = Vertex(2.5f, -2.5f, -2.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[16] = Vertex(2.5f, 2.5f, 2.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[17] = Vertex(2.5f, -2.5f, 2.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	// mirror (back)
	v[18] = Vertex(-2.5f, -2.5f, -2.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[19] = Vertex(-2.5f, 2.5f, -2.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[20] = Vertex(2.5f, 2.5f, -2.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);

	v[21] = Vertex(-2.5f, -2.5f, -2.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[22] = Vertex(2.5f, 2.5f, -2.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[23] = Vertex(2.5f, -2.5f, -2.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	// mirror (top)
	v[24] = Vertex(2.5f, 2.5f, 2.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);//Close Left
	v[25] = Vertex(2.5f, 2.5f, -2.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);//Far Left
	v[26] = Vertex(-2.5f, 2.5f, -2.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);//Far Right

	v[27] = Vertex(2.5f, 2.5f, 2.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);//Close Left
	v[28] = Vertex(-2.5f, 2.5f, -2.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);//Far Right
	v[29] = Vertex(-2.5f, 2.5f, 2.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);//Close Right

	// mirror (bottom)
	v[30] = Vertex(2.5f, -2.5f, -2.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);//Far Left
	v[31] = Vertex(2.5f, -2.5f, 2.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);//Close Left
	v[32] = Vertex(-2.5f, -2.5f, 2.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);//Close Right

	v[33] = Vertex(2.5f, -2.5f, -2.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);//Far Left
	v[34] = Vertex(-2.5f, -2.5f, 2.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);//Close Right
	v[35] = Vertex(-2.5f, -2.5f, -2.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);//Far Right

	mirrorVertices->Unlock();

	//
	// Load Textures, set filters.
	//

	D3DXCreateTextureFromFile(g_pDevice, "ice.bmp", &MirrorTex);

	g_pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
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
	Release<LPDIRECT3DVERTEXBUFFER9>(mirrorVertices);
	Release<IDirect3DTexture9*>(MirrorTex);
	Delete<PSystem*>(Sno);
}

void Graphics::Render(std::vector<Entity*> entities, std::vector<Mirror*> mirrors)
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
			}
		}
	}

	//---DRAW MIRRORS BLOCK---//
	//Draw Mirror Cube
	DrawCube();

	//Calculate Cam Vector
	double yWeakness = 1 - pow(fabs(cam_angle - 0.5) * 2, 2);
	D3DXVECTOR3 vLookatPt((FLOAT)sin(cam_spin * D3DX_PI * 2) * yWeakness, (FLOAT)cos(cam_angle * D3DX_PI), (FLOAT)sin((cam_spin - 0.25) * D3DX_PI * 2) * yWeakness);


	g_pDevice->Clear(0, 0, D3DCLEAR_STENCIL, 0xff000000, 1.0f, 0L);
	int i = 1;
	//Draw Mirrors to Stencil Buffer
	for (std::vector<Mirror*>::iterator it = mirrors.begin(); it != mirrors.end(); ++it) {
		RenderMirror((*it), entities, vLookatPt.x, vLookatPt.y, vLookatPt.z, i++);
	}
	i = 1;
	//Draw Reflections
	for (std::vector<Mirror*>::iterator it = mirrors.begin(); it != mirrors.end(); ++it) {
		RenderReflection((*it), entities, vLookatPt.x, vLookatPt.y, vLookatPt.z, i++);
	}

	//---DRAW PARTICLES BLOCK---//
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

void Graphics::RenderMirror(Mirror* m, std::vector<Entity*> entities, float camV_x, float camV_y, float camV_z, int stencilNumber)
{
	//
	// Draw Mirror quad to stencil buffer ONLY.  In this way
	// only the stencil bits that correspond to the mirror will
	// be on.  Therefore, the reflected teapot can only be rendered
	// where the stencil bits are turned on, and thus on the mirror 
	// only.

	g_pDevice->SetRenderState(D3DRS_STENCILENABLE, true);
	g_pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
	g_pDevice->SetRenderState(D3DRS_STENCILREF, stencilNumber);
	g_pDevice->SetRenderState(D3DRS_STENCILMASK, 0xffffffff);
	g_pDevice->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);
	g_pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
	g_pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
	g_pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);

	// disable writes to the depth and back buffers
	g_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);
	g_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	g_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
	g_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// draw the mirror to the stencil buffer
	g_pDevice->SetStreamSource(0, mirrorVertices, 0, sizeof(Vertex));
	g_pDevice->SetFVF(Vertex::FVF);
	g_pDevice->SetMaterial(&MirrorMtrl);
	g_pDevice->SetTexture(0, MirrorTex);
	D3DXMATRIX I;
	D3DXMatrixIdentity(&I);
	g_pDevice->SetTransform(D3DTS_WORLD, &I);
	g_pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, m->rN(), 2);

	// re-enable depth writes
	g_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);

	// Restore render states.
	g_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	g_pDevice->SetRenderState(D3DRS_STENCILENABLE, false);
	g_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void Graphics::RenderReflection(Mirror* m, std::vector<Entity*> entities, float camV_x, float camV_y, float camV_z, int stencilNumber)
{
	//
	// Draw Mirror quad to stencil buffer ONLY.  In this way
	// only the stencil bits that correspond to the mirror will
	// be on.  Therefore, the reflected teapot can only be rendered
	// where the stencil bits are turned on, and thus on the mirror 
	// only.

	g_pDevice->SetRenderState(D3DRS_STENCILENABLE, true);
	g_pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
	g_pDevice->SetRenderState(D3DRS_STENCILREF, stencilNumber);
	g_pDevice->SetRenderState(D3DRS_STENCILMASK, 0xffffffff);
	g_pDevice->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);
	g_pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
	g_pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
	g_pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);

	// disable writes to the depth and back buffers
	g_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	g_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
	g_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// only draw reflected teapot to the pixels where the mirror was drawn to.
	g_pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
	g_pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);

	// position reflection
	D3DXMATRIX W, T, R, MT;
	D3DXPLANE plane(m->px(), m->py(), m->pz(), 0.0f); // xy plane
	D3DXMatrixReflect(&R, &plane);

	for (std::vector<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it) {
		if ((*it)->IsDrawable3D())
		{

			D3DXMATRIXA16 matWorld_YPR;
			D3DXMatrixRotationYawPitchRoll(&matWorld_YPR, (FLOAT)(*it)->GetYaw(), (FLOAT)(*it)->GetPitch(), (FLOAT)(*it)->GetRoll());
			//MT is the Mirror Transition. That is: the distance the object is relative to the mirror. Tip->Tail, Obj->Mirror
			float mtx = Sign(m->px()) == Sign((*it)->GetX() - m->mx()) ? ((*it)->GetX() - m->mx()) * m->px() * Sign((*it)->GetX() - m->mx()) : 0;
			float mty = Sign(m->py()) == Sign((*it)->GetY() - m->my()) ? ((*it)->GetY() - m->my()) * m->py() * Sign((*it)->GetY() - m->my()) : 0;
			float mtz = Sign(m->pz()) == Sign((*it)->GetZ() - m->mz()) ? ((*it)->GetZ() - m->mz()) * m->pz() * Sign((*it)->GetZ() - m->mz()) : 0;

			D3DXMatrixTranslation(&MT,
				mtx,
				mty,
				mtz);

			D3DXMatrixTranslation(&T,
				(*it)->GetX(),
				(*it)->GetY(),
				(*it)->GetZ());

			W = matWorld_YPR * MT * R * T;

			// clear depth buffer and blend the reflected teapot with the mirror
			g_pDevice->Clear(0, 0, D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
			g_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_DESTCOLOR);
			g_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);

			if ((mtx || mty || mtz) && camV_x * m->px() + camV_y * m->py() + camV_z * m->pz() < 0)
			{
				// Finally, draw the reflected teapot
				// Meshes are divided into subsets, one for each material. Render them in a loop
				for (DWORD i = 0; i < (*it)->GetModel()->NumMaterials(); i++)
				{
					// Set the material and texture for this subset
					g_pDevice->SetTransform(D3DTS_WORLD, &W);
					g_pDevice->SetMaterial(&((*it)->GetModel()->GetMaterial())[i]);
					g_pDevice->SetTexture(0, (*it)->GetModel()->GetTexture()[i]);

					// Draw the mesh subset
					g_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
					(*it)->GetModel()->GetMesh()->DrawSubset(i);
				}
			}
		}
	}

	// Restore render states.
	g_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	g_pDevice->SetRenderState(D3DRS_STENCILENABLE, false);
	g_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void Graphics::DrawCube()
{
	D3DXMATRIX I;
	D3DXMatrixIdentity(&I);
	g_pDevice->SetTransform(D3DTS_WORLD, &I);

	g_pDevice->SetStreamSource(0, mirrorVertices, 0, sizeof(Vertex));
	g_pDevice->SetFVF(Vertex::FVF);

	// draw the mirror
	g_pDevice->SetMaterial(&MirrorMtrl);
	g_pDevice->SetTexture(0, MirrorTex);
	g_pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 30, 2);
	g_pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 24, 2);
	g_pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 18, 2);
	g_pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 12, 2);
	g_pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 6, 2);
	g_pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);
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