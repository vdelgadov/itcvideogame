#include "Effect.h"
Effect::Effect(CEngine* engine)
{
	Effect::Effect(engine, BLINN_PHONG);
}
Effect::Effect(CEngine* engine, int effect_type)
{
	this->engine=engine;
//	Effect::initializeShaders();
	Effect::buildFX(effect_type);
}
Effect::~Effect()
{
	//Nothing to do here
}

void Effect::buildFX(int effect_type)
{
	this->effect_type=effect_type;
	switch(this->effect_type)
	{
	case BLINN_PHONG:

		break;
	case BUMP_MAPPING:
		ID3DXBuffer* errors = 0;
		D3DXCreateEffectFromFile(this->engine->d3ddev, L"NormalMap.fx", 
			0, 0, D3DXSHADER_DEBUG, 0, &this->mFX, &errors);
		if( errors )
			MessageBox(0, (LPCWSTR)errors->GetBufferPointer(), 0, 0);

		// Obtain handles.
		mhTech       = mFX->GetTechniqueByName("NormalMapTech");
		mhWVP        = mFX->GetParameterByName(0, "gWVP"); //World View Projection
		mhWorldInv   = mFX->GetParameterByName(0, "gWorldInv"); //World Inverse
		mhMtrl       = mFX->GetParameterByName(0, "gMtrl"); //Matrix Lights
		mhLight      = mFX->GetParameterByName(0, "gLight"); //Light Direction
		mhEyePosW    = mFX->GetParameterByName(0, "gEyePosW"); //Eye  Position
		mhTex        = mFX->GetParameterByName(0, "gTex"); //Texture
		mhNormalMap  = mFX->GetParameterByName(0, "gNormalMap"); //Normal Map	
		mFX->SetTechnique(mhTech);
		break;
	}
}
Effect::renderObject()



