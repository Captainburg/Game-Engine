#include "Sprite.h"
#include <cmath>

Sprite::Sprite(char filename[])
	: fname(filename)
	, bSurfaceLoaded(false)
{
	bitmap = (BITMAP*)malloc(sizeof(BITMAP));
	HBITMAP hBM = (HBITMAP)LoadImage(NULL, fname, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	GetObject(hBM, sizeof(BITMAP), bitmap);
	FlipSprite();
}

Sprite::~Sprite()
{
	if (bitmap)
	{
		free(bitmap);
	}
	if (IsSurfaceLoaded())
	{
		pSurface->Release();
	}
}

void Sprite::FlipSprite()
{
	//Get Variables we need
	int pixSize = bitmap->bmBitsPixel / 8;

	//Pointer to ImageTracers
	byte* imgPtrOld = static_cast<byte*>(bitmap->bmBits);
	byte* imgPtrNew = (byte*)malloc(3 * bitmap->bmWidth * bitmap->bmHeight);

	for (int nY = 0; nY < bitmap->bmHeight; nY++)
	{
		for (int nX = 0; nX < bitmap->bmWidth; nX++)
		{
			//Find Next New Pixel
			int ptrOffsetInverse = (bitmap->bmHeight - nY - 1) * pixSize * bitmap->bmWidth //Row
				+ nX * pixSize;				 //Column

			int ptrOffset = nY * pixSize * bitmap->bmWidth //Row
				+ nX * pixSize;				 //Column

			//Assign Pixel
			imgPtrNew[ptrOffsetInverse + 0] = imgPtrOld[ptrOffset + 0];
			imgPtrNew[ptrOffsetInverse + 1] = imgPtrOld[ptrOffset + 1];
			imgPtrNew[ptrOffsetInverse + 2] = imgPtrOld[ptrOffset + 2];
		}
	}

	//Change BITMAP* Info
	bitmap->bmBits = imgPtrNew;
}

void Sprite::ResizeSprite(int X, int Y)
{
	//Get Variables we need
	int pixSize = bitmap->bmBitsPixel / 8;

	//Pointer to ImageTracers
	byte* imgPtrOld = static_cast<byte*>(bitmap->bmBits);
	byte* imgPtrNew = (byte*)malloc(3 * X * Y);

	//Ratio New:Old
	float xRatio = static_cast<float>(bitmap->bmWidth) / X;
	float yRatio = static_cast<float>(bitmap->bmHeight) / Y;

	for (int nY = 0; nY < Y; nY++)
	{
		for (int nX = 0; nX < X; nX++)
		{
			//Find Next New Pixel
			int ptrOffset = nY * pixSize * X //Row
						  + nX * pixSize;				   //Column

			//Find Next Old Pixel
			int oldOffset = static_cast<int>(floor(yRatio * nY)) * pixSize * bitmap->bmWidth //Row
						  + static_cast<int>(floor(xRatio * nX)) * pixSize;				   //Column

			//Assign Pixel
			imgPtrNew[ptrOffset + 0] = imgPtrOld[oldOffset + 0];
			imgPtrNew[ptrOffset + 1] = imgPtrOld[oldOffset + 1];
			imgPtrNew[ptrOffset + 2] = imgPtrOld[oldOffset + 2];
		}
	}

	//Change BITMAP* Info
	bitmap->bmWidth = X;
	bitmap->bmHeight = Y;
	bitmap->bmBitsPixel = 24;
	bitmap->bmWidthBytes = X * 3;
	bitmap->bmBits = imgPtrNew;
}

BITMAP* Sprite::GetBitmap()
{
	return bitmap;
}

LPDIRECT3DSURFACE9 Sprite::GetSurface()
{
	return pSurface;
}

char * Sprite::GetFileName()
{
	return fname;
}

void Sprite::GiveSurfacePointer(LPDIRECT3DSURFACE9 newSurf)
{
	bSurfaceLoaded = true;
	pSurface = newSurf;
}

bool Sprite::IsSurfaceLoaded()
{
	return bSurfaceLoaded;
}
