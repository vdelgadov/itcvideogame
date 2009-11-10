#include "Engine.h"
#include "Graphics.h"
#include "Params.h"

#define BLINN_PHONG 1
#define BUMP_MAPPING 2

class Effect
{
private:
	ID3DXEffect* mFX;
	CEngine* engine;
public:
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
	DirLight mLight;	//Struct para todos  los tipos de luces
	void buildFX(int effect_type);
	void renderObject();
};