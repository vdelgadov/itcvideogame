#include "Effect.h"
Effect::Effect()
{
}
Effect::Effect(CEngine* engine)
{
	Effect::Effect(engine, BLINN_PHONG);
}
Effect::Effect(CEngine* engine, int effect_type)
{
	this->engine=engine;
	if ( FAILED (D3DXCreateTextureFromFileA( this->engine->d3ddev,"white.jpg",&mWhiteTex)))
	{
			cout << "error al crear textura" << endl ;
	}



//	Effect::initializeShaders();
	Effect::buildFX(effect_type);
}
Effect::~Effect()
{
	//Nothing to do here
}

void Effect::buildFX(int effect_type)
{
		mLight.ambient = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f);
		mLight.diffuse = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
		mLight.spec    = D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f);
		mLight.dirW.x = 0.0f;
		mLight.dirW.y = -1.0f;
		mLight.dirW.z = 5.0f;
		this->engine->matView;


	this->effect_type=effect_type;
	switch(this->effect_type)
	{
	case BLINN_PHONG:

		break;
	case BUMP_MAPPING:
		{
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

case SPECULAR_MAPPING:
	{
		ID3DXBuffer* errors = 0;
		D3DXCreateEffectFromFile(this->engine->d3ddev, L"NormalMapSpec.fx", 
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
		mhSpecularMap  = mFX->GetParameterByName(0, "gSpecMap"); //Specular Map	
		mFX->SetTechnique(mhTech);
		break;
	}
	}
}
void Effect::renderObject(LPD3DXMESH pMesh,
						  DWORD dwNumMaterials,
						  std::vector<Mtrl> mShaderMtrls,
						  LPDIRECT3DTEXTURE9* pMeshTextures,
						  LPDIRECT3DTEXTURE9* pMeshNormalMapTextures,
						  LPDIRECT3DTEXTURE9* pMeshSpecularMapTextures)
{
	switch(this->effect_type)
	{
	
		case BUMP_MAPPING:
			{
			D3DXMATRIX mSceneWorldInv;
			D3DXMATRIX matWorld,matView,matProj, iv;
			engine->d3ddev->GetTransform(D3DTS_WORLD,&matWorld);
			engine->d3ddev->GetTransform(D3DTS_VIEW,&matView);
			engine->d3ddev->GetTransform(D3DTS_PROJECTION,&matProj);
			D3DXMatrixInverse( &iv, NULL, &matView );
			eyePosition.x = iv._41;
			eyePosition.y = iv._42;
			eyePosition.z = iv._43;


			D3DXVec3Normalize(&mLight.dirW,&mLight.dirW);
			D3DXVec3Normalize(&eyePosition,&eyePosition);
			D3DXMATRIX matWorldViewProj = matWorld*matView*matProj;
			D3DXMatrixInverse(&mSceneWorldInv,NULL,&matWorld);

			mFX->SetValue(mhLight, &mLight, sizeof(DirLight));
			mFX->SetMatrix(mhWVP, &(matWorldViewProj));
			mFX->SetMatrix(mhWorldInv, &mSceneWorldInv);
			mFX->SetValue(mhEyePosW, &eyePosition, sizeof(D3DXVECTOR3));
		
			UINT numPasses = 0;
			mFX->Begin(&numPasses, 0);
			for( UINT j =0; j <numPasses ;j++)
			{
				mFX->BeginPass(j);
			for( DWORD i = 0; i < dwNumMaterials; i++ )
			{	
				
				//engine->d3ddev->SetMaterial( &pMeshMaterials[i] );
				//engine->d3ddev->SetTexture( 0, pMeshTextures[i] );
				mFX->SetValue(mhMtrl, &mShaderMtrls[i], sizeof(Mtrl));
				// If there is a texture, then use.
				
				if(pMeshTextures[i] != 0)
				{
					mFX->SetTexture(mhTex, pMeshTextures[i]);
				}
				else
				{
					mFX->SetTexture(mhTex, mWhiteTex);
				}
				if(pMeshNormalMapTextures[i] != 0)
				{
					mFX->SetTexture(mhNormalMap, pMeshNormalMapTextures[i]);
				}
				else
				{
					mFX->SetTexture(mhNormalMap, mWhiteTex);
				}
				mFX->CommitChanges();
				pMesh->DrawSubset( i );
			}
			mFX->EndPass();
			}
			mFX->End();
			
			break;
			}
		case SPECULAR_MAPPING:
			{
			D3DXMATRIX mSceneWorldInv;
			D3DXMATRIX matWorld,matView,matProj;

			engine->d3ddev->GetTransform(D3DTS_WORLD,&matWorld);
			engine->d3ddev->GetTransform(D3DTS_VIEW,&matView);
			engine->d3ddev->GetTransform(D3DTS_PROJECTION,&matProj);

			D3DXVec3Normalize(&mLight.dirW,&mLight.dirW);
			D3DXVec3Normalize(&eyePosition,&eyePosition);
			D3DXMATRIX matWorldViewProj = matWorld*matView*matProj;
			D3DXMatrixInverse(&mSceneWorldInv,NULL,&matWorld);

			mFX->SetValue(mhLight, &mLight, sizeof(DirLight));
			mFX->SetMatrix(mhWVP, &(matWorldViewProj));
			mFX->SetMatrix(mhWorldInv, &mSceneWorldInv);
			mFX->SetValue(mhEyePosW, &eyePosition, sizeof(D3DXVECTOR3));
		
			UINT numPasses = 0;
			mFX->Begin(&numPasses, 0);
			for( UINT j =0; j <numPasses ;j++)
			{
				mFX->BeginPass(j);
			
			for( DWORD i = 0; i < dwNumMaterials; i++ )
			{		
				mFX->SetValue(mhMtrl, &mShaderMtrls[i], sizeof(Mtrl));
				
				if(pMeshTextures[i] != 0)
				{
					mFX->SetTexture(mhTex, pMeshTextures[i]);
				}
				else
				{
					mFX->SetTexture(mhTex, mWhiteTex);
				}
				if(pMeshNormalMapTextures[i] != 0)
				{
					mFX->SetTexture(mhNormalMap, pMeshNormalMapTextures[i]);
				}
				else
				{
					mFX->SetTexture(mhNormalMap, mWhiteTex);
				}

				if(pMeshSpecularMapTextures[i] != 0)
				{
				
					mFX->SetTexture(mhSpecularMap, pMeshSpecularMapTextures[i]);
				}
				else
				{
					mFX->SetTexture(mhSpecularMap, mWhiteTex);
				}

				mFX->CommitChanges();
				pMesh->DrawSubset( i );
			}
			mFX->EndPass();
			}
			mFX->End();
			break;
			}
	}

}