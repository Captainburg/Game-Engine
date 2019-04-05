#include "Font.h"
#include <windows.h>
#include <tchar.h>
#include <stdio.h>

Font::Font(LPDIRECT3DDEVICE9 g_pDevice)
	: g_AlphabetWidth(0)
	, g_AlphabetHeight(0)
	, g_AlphabetLetterWidth(0)
	, g_AlphabetLetterHeight(0)
	, g_AlphabetLettersPerRow(0)
	, g_pAlphabetSurface(0)
{
	this->g_pDevice = g_pDevice;
}

Font::~Font()
{
}

void Font::DrawString(int x, int y, const char* str, bool bTransparent, D3DCOLOR ColorKey, DWORD* pDestData, int DestPitch)
{
	// Loop for each character in the string
	for (UINT i = 0; i < strlen(str); i++)
	{
		DrawChar(x + (g_AlphabetLetterWidth * i), y, str[i], bTransparent, ColorKey, pDestData, DestPitch);
	}
}

void Font::DrawChar(int x, int y, char Character, bool bTransparent, D3DCOLOR ColorKey, DWORD* pDestData, int DestPitch)
{
	div_t Result;

	int OffsetX = 0, OffsetY = 0;

	POINT LetterDestPoint = { 0, 0 };
	RECT LetterRect = { 0, 0, 0, 0 };

	Character -= 32;

	if (Character == 0)
		return;

	Result = div(Character, g_AlphabetLettersPerRow);
	OffsetX = Result.rem * g_AlphabetLetterWidth;
	OffsetY = Result.quot * g_AlphabetLetterHeight;

	// Fill in the source rectangle with the computed offsets
	SetRect(&LetterRect, OffsetX, OffsetY, OffsetX + g_AlphabetLetterWidth, OffsetY + g_AlphabetLetterHeight);

	// Fill in the destination point
	LetterDestPoint.x = x;
	LetterDestPoint.y = y;

	D3DLOCKED_RECT LockedAlphabet;

	g_pAlphabetSurface->LockRect(&LockedAlphabet, 0, D3DLOCK_READONLY);

	// Get a DWORD pointer to each surface
	DWORD* pAlphaData = (DWORD*)LockedAlphabet.pBits;

	// Convert the BYTE pitch pointer to a DWORD ptr
	LockedAlphabet.Pitch /= 4;
	DestPitch /= 4;

	// Compute the offset into the alphabet
	int AlphaOffset = OffsetY * LockedAlphabet.Pitch + OffsetX;
	// Compute the offset into the destination surface
	int DestOffset = y * DestPitch + x;

	// Loop for each row in the letter
	for (int cy = 0; cy < g_AlphabetLetterHeight; cy++)
	{
		// Loop for each column in the letter
		for (int cx = 0; cx < g_AlphabetLetterWidth; cx++)
		{
			if (bTransparent)
			{
				// If this alphabet pixel is not transparent
				if (pAlphaData[AlphaOffset] != ColorKey)
				{
					// Then copy the pixel to the destination
					pDestData[DestOffset] = pAlphaData[AlphaOffset];
				}

				// Increment the offsets to the next pixel
				AlphaOffset++;
				DestOffset++;
			}
			else
				pDestData[DestOffset] = pAlphaData[AlphaOffset];
		}

		// Move the offsets to the start of the next row
		DestOffset += DestPitch - g_AlphabetLetterWidth;
		AlphaOffset += LockedAlphabet.Pitch - g_AlphabetLetterWidth;
	}

	// Unlock the surface
	g_pAlphabetSurface->UnlockRect();
}

HRESULT Font::LoadAlphabet(char* strPathName, int LetterWidth, int LetterHeight)
{
	// Make sure a valid path was specified
	if (!strPathName)
		return E_FAIL;

	// Make sure the size of the letters is greater than 0
	if (!LetterWidth || !LetterHeight)
		return E_FAIL;

	// Load the bitmap into memory
	this->LoadAlphabetToSurface(strPathName, &g_pAlphabetSurface, g_pDevice);

	// Holds information about the alphabet surface
	D3DSURFACE_DESC d3dsd;

	// Get information about the alphabet surface
	g_pAlphabetSurface->GetDesc(&d3dsd);

	// Update globals with the letter dimensions
	g_AlphabetWidth = d3dsd.Width;
	g_AlphabetHeight = d3dsd.Height;
	g_AlphabetLetterWidth = LetterWidth;
	g_AlphabetLetterHeight = LetterHeight;

	// Compute the number of letters in a row
	g_AlphabetLettersPerRow = g_AlphabetWidth / g_AlphabetLetterWidth;

	return S_OK;
}

HRESULT Font::UnloadAlphabet()
{
	// Check if the alphabet exists
	if (g_pAlphabetSurface)
	{
		// Release the surface
		g_pAlphabetSurface->Release();
		// NULL the pointer
		g_pAlphabetSurface = 0;
	}

	return S_OK;
}

int Font::LoadAlphabetToSurface(char* PathName, LPDIRECT3DSURFACE9* ppSurface, LPDIRECT3DDEVICE9 pDevice) {

	HBITMAP hBitmap;
	BITMAP Bitmap;

	hBitmap = (HBITMAP)LoadImage(NULL, PathName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	GetObject(hBitmap, sizeof(BITMAP), &Bitmap);
	DeleteObject(hBitmap);//we only needed it for the header info to create a D3D surface

	//create surface for bitmap
	pDevice->CreateOffscreenPlainSurface(Bitmap.bmWidth, Bitmap.bmHeight, D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH, ppSurface, NULL);

	//pDevice->CreateImageSurface(Bitmap.bmWidth, Bitmap.bmHeight, D3DFMT_A8R8G8B8, ppSurface);

	//load bitmap onto surface
	D3DXLoadSurfaceFromFile(*ppSurface, NULL, NULL, PathName, NULL, D3DX_FILTER_NONE, 0, NULL);

	return S_OK;
}