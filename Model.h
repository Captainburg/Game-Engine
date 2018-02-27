#pragma once
#include <d3dx9.h>

class Model {
public:
	Model(char filename[], LPDIRECT3DDEVICE9 g_pDevice);
	virtual ~Model();

	/** Get Filename Pointer
	*
	* Get a Filename pointer.
	*
	* @return char* a Filename pointer.
	*/
	char* GetFileName();
	/** Get Mesh
	*
	* Get a Mesh.
	*
	* @return LPD3DXMESH a mesh.
	*/
	LPD3DXMESH GetMesh();
	/** Get Material
	*
	* Get a Material.
	*
	* @return D3DMATERIAL9* a Material pointer.
	*/
	D3DMATERIAL9* GetMaterial();
	/** Get Texture
	*
	* Get a Texture.
	*
	* @return LPDIRECT3DTEXTURE9* a Texture pointer.
	*/
	LPDIRECT3DTEXTURE9* GetTexture();
	/** Get Num of Materials
	*
	* Get the Num of Materials.
	*
	* @return DWORD the Num of Materials.
	*/
	DWORD NumMaterials();

private:
	char* fname;
	LPD3DXMESH g_pMesh;
	D3DMATERIAL9* g_pMeshMaterials;
	LPDIRECT3DTEXTURE9* g_pMeshTextures;
	DWORD g_dwNumMaterials;
};