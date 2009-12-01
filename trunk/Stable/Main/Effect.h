#pragma once
#include "Engine.h"
#include "Graphics.h"
#include "Params.h"
#include "CObject.h"

#define BLINN_PHONG 1
#define BUMP_MAPPING 2
#define SPECULAR_MAPPING 3

class Effect
{
private:
	ID3DXEffect* mFX;
	CEngine* engine;
public:
	Effect();
	Effect(CEngine* engine);
	Effect(CEngine* engine, int effect_type);
	~Effect();
	int effect_type;
	D3DXHANDLE   mhTech; //Handler de la tecnica
	D3DXHANDLE   mhWVP; //Handler World View Projection
	D3DXHANDLE   mhWorldInv; //Handler Matriz World Inverse
	D3DXHANDLE   mhEyePosW; //Handler de la posición del ojo
	D3DXHANDLE   mhTex; //Handler de la textura
	D3DXHANDLE   mhMtrl; //Handler del material
	D3DXHANDLE   mhLight; //Handler de TODAS las luces
	D3DXHANDLE   mhNormalMap; //Handler para el mapa normal
	D3DXHANDLE   mhSpecularMap;
	D3DXVECTOR3 eyePosition;
	IDirect3DTexture9* mWhiteTex;
	DirLight mLight;	//Struct para todos  los tipos de luces
	void buildFX(int effect_type);
	void renderObject(  LPD3DXMESH pMesh,
						DWORD dwNumMaterials,
						std::vector<Mtrl> mShaderMtrls,
						LPDIRECT3DTEXTURE9* pMeshTextures,
						LPDIRECT3DTEXTURE9* pMeshNormalMapTextures,
						LPDIRECT3DTEXTURE9* pMeshSpecularMapTextures);
};