#pragma once
#include <d3d9.h>
#include <d3dx9.h>

class Font {
public:
	Font(LPDIRECT3DDEVICE9 g_pDevice);
	virtual ~Font();

	/** Load Alphabet
	*
	* Loads the Alphabet.
	*
	* @param strPathName The path to the font file.
	* @param LetterWidth The width of the Font.
	* @param LetterHeight The height of the Font.
	* @return HRESULT The result of loading the alphabet - whether it was successful.
	*/
	HRESULT LoadAlphabet(char* strPathName, int LetterWidth, int LetterHeight);
	/** Unload Alphabet
	*
	* Unloads the Alphabet from memory.
	*
	* @return HRESULT The result of unloading the alphabet - whether it was successful.
	*/
	HRESULT UnloadAlphabet();
	/** Draw String
	*
	* Calls DrawChar until a String has been drawn.
	*
	* @param x The x co-ordinate to draw at.
	* @param y The y co-ordinate to draw at.
	* @param String The String to draw.
	* @param bTransparent Whether the text is transparent.
	* @param ColorKey The color of the background to ignore.
	* @param pDestData The surface.
	* @param DestPitch The width of the surface in bytes.
	* @return null
	*/
	void DrawString(int x, int y, const char* String, bool bTransparent, D3DCOLOR ColorKey, DWORD* pDestData, int DestPitch);

private:
	/** Draw Char
	*
	* Draws a character.
	*
	* @param x The x co-ordinate to draw at.
	* @param y The y co-ordinate to draw at.
	* @param Character The Character to draw.
	* @param bTransparent Whether the text is transparent.
	* @param ColorKey The color of the background to ignore.
	* @param pDestData The surface.
	* @param DestPitch The width of the surface in bytes.
	* @return null
	*/
	void DrawChar(int x, int y, char Character, bool bTransparent, D3DCOLOR ColorKey, DWORD* pDestData, int DestPitch);
	/** Load Alphabet to Surface
	*
	* Loads the Alphabet to the Surface.
	*
	* @param strPathName The path to the font file.
	* @param ppSurface A double pointer to the surface.
	* @param pDevice The device to load it to.
	* @return int The result of loading the alphabet - whether it was successful.
	*/
	int LoadAlphabetToSurface(char * PathName, LPDIRECT3DSURFACE9 * ppSurface, LPDIRECT3DDEVICE9 pDevice);

	int g_AlphabetWidth;
	int g_AlphabetHeight;
	int g_AlphabetLetterWidth;
	int g_AlphabetLetterHeight;
	int g_AlphabetLettersPerRow;
	LPDIRECT3DSURFACE9 g_pAlphabetSurface;
	LPDIRECT3DDEVICE9 g_pDevice;
};