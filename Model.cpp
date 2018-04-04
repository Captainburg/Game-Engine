#include "Model.h"

Model::Model(char filename[], LPDIRECT3DDEVICE9 g_pDevice)
	: fname(filename)
	, g_pMesh(NULL)
	, g_pMeshMaterials(NULL)
	, g_pMeshTextures(NULL)
	, g_dwNumMaterials(0L)
{
	LPD3DXBUFFER pD3DXMtrlBuffer;

	// Load the mesh from the specified file
	D3DXLoadMeshFromX(filename, D3DXMESH_SYSTEMMEM, g_pDevice, NULL, &pD3DXMtrlBuffer, NULL, &g_dwNumMaterials, &g_pMesh);

	// We need to extract the material properties and texture names from the pD3DXMtrlBuffer
	D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
	g_pMeshMaterials = new D3DMATERIAL9[g_dwNumMaterials];
	g_pMeshTextures = new LPDIRECT3DTEXTURE9[g_dwNumMaterials];

	for (DWORD i = 0; i < g_dwNumMaterials; i++)
	{
		// Copy the material
		g_pMeshMaterials[i] = d3dxMaterials[i].MatD3D;

		// Set the ambient color for the material (D3DX does not do this)
		g_pMeshMaterials[i].Ambient = g_pMeshMaterials[i].Diffuse;

		g_pMeshTextures[i] = NULL;
		if (d3dxMaterials[i].pTextureFilename != NULL && lstrlen(d3dxMaterials[i].pTextureFilename) > 0)
		{
			// Create the texture
			D3DXCreateTextureFromFile(g_pDevice, d3dxMaterials[i].pTextureFilename, &g_pMeshTextures[i]);
		}
	}
	//
	// Compute the bounding sphere.
	//
	BYTE* v = 0;
	g_pMesh->LockVertexBuffer(0, (void**)&v);

	D3DXComputeBoundingSphere(
		(D3DXVECTOR3*)v,
		g_pMesh->GetNumVertices(),
		D3DXGetFVFVertexSize(g_pMesh->GetFVF()),
		&BSphere._center,
		&BSphere._radius);

	g_pMesh->UnlockVertexBuffer();

	D3DXCreateSphere(g_pDevice, BSphere._radius, 20, 20, &Sphere, 0);

	// Done with the material buffer
	pD3DXMtrlBuffer->Release();
}

Model::~Model()
{
	if (g_pMeshMaterials != NULL)
	{
		delete[] g_pMeshMaterials;
	}

	if (g_pMeshTextures)
	{
		for (DWORD i = 0; i < g_dwNumMaterials; i++)
		{
			if (g_pMeshTextures[i])
				g_pMeshTextures[i]->Release();
		}
		delete[] g_pMeshTextures;
	}
	if (g_pMesh != NULL)
	{
		Sphere->Release();
		g_pMesh->Release();
	}
}

char * Model::GetFileName()
{
	return fname;
}

LPD3DXMESH Model::GetMesh()
{
	return g_pMesh;
}

LPD3DXMESH Model::GetSphere()
{
	return Sphere;
}

BoundingSphere* Model::GetBSphere()
{
	return &BSphere;
}

D3DMATERIAL9 * Model::GetMaterial()
{
	return g_pMeshMaterials;
}

LPDIRECT3DTEXTURE9 * Model::GetTexture()
{
	return g_pMeshTextures;
}

DWORD Model::NumMaterials()
{
	return g_dwNumMaterials;
}
