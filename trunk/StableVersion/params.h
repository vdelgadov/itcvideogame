#include <vector>
#include <list>
#include "Engine.h"
using namespace std;


class CObject
{
public:
	//ID
	int ID;
	D3DXMATRIX translation, rotation, scale;
	//file
	LPCTSTR fileName;
	//Engine
	CEngine* engine;
	
	CObject* pParent;
	//render variables todos null
	LPD3DXMESH				pMesh; // Our mesh object in sysmem
	D3DMATERIAL9*			pMeshMaterials; // Materials for our mesh
	LPDIRECT3DTEXTURE9*		pMeshTextures; // Textures for our mesh
	DWORD					dwNumMaterials; // Number of mesh materials
	LPD3DXBUFFER			pD3DXMtrlBuffer;

	//Child list and parent
	list<CObject*> lstChilds;

public:	
	CObject()
	{
		//id
		this->ID = ID;
		D3DXMatrixTranslation(&translation,0.0,0.0,0.0);
		D3DXMatrixRotationYawPitchRoll(&rotation, 0.0, 0.0, 0.0);
		D3DXMatrixScaling(&scale, 1.0, 1.0, 1.0);

		//file
		this->fileName = NULL;
		//parent
		this->pParent = NULL;
	}
	CObject(int ID,float fPosX, float fPosY, float fPosZ, float fRotX, float fRotY, float fRotZ,float fScaleX,float fScaleY,float fScaleZ, LPCTSTR fileName, CObject* parent,CEngine* engine)
	{
		//id
		this->ID = ID;

		D3DXMatrixTranslation(&translation,fPosX,fPosY,fPosZ);
		D3DXMatrixScaling(&scale, fScaleX, fScaleY, fScaleZ);
		D3DXMatrixRotationYawPitchRoll(&rotation, fRotX, fRotY, fRotZ);

		//file
		this->fileName = fileName;
		//parent
		this->pParent = parent;
		this->engine = engine;
		this->initializeMesh();
	}
	virtual ~CObject() {
		for(list<CObject*>::iterator it = lstChilds.begin(); it != lstChilds.end(); ++it) {
			delete *it;
		}
	}
	virtual void render()
	{
		engine->d3ddev->SetTransform(D3DTS_WORLD,&( (this->scale) * (this->rotation) * (this->translation)) ); //

        for( DWORD i = 0; i < dwNumMaterials; i++ )
        {		
            engine->d3ddev->SetMaterial( &pMeshMaterials[i] );
            engine->d3ddev->SetTexture( 0, pMeshTextures[i] );

            // Draw the mesh subset
            pMesh->DrawSubset( i );
        }
		//render all childs
		for(list<CObject*>::iterator it = lstChilds.begin(); it != lstChilds.end(); ++it)
		{
			(*it)->render();
		}

	}
	void AddChild(CObject* pNode)
	{
		lstChilds.push_back(pNode);
		pNode->translation = pNode->pParent->translation * pNode->translation;
		pNode->rotation = pNode->pParent->rotation * pNode->rotation;
		pNode->scale = pNode->pParent->scale * pNode->scale;
	}
	CObject* find(int id)
	{
		for(list<CObject*>::iterator it = lstChilds.begin(); it != lstChilds.end(); ++it) {
			CObject* o = *it;
			if( o->ID == id)
			{
				return o;
			}
			else
			{
				return (*it)->find(id);
			}
		}
		return NULL;
	}
	void move(float tX, float tY, float tZ, float rX, float rY, float rZ, float sX, float sY, float sZ)
	{
		D3DXMATRIX translationTemp, rotationTemp, scaleTemp;

		D3DXMatrixTranslation(&translationTemp,tX,tY,tZ);
		D3DXMatrixRotationYawPitchRoll(&rotationTemp, rX, rY, rZ);
		D3DXMatrixScaling(&scaleTemp, sX, sY, sZ);

		this->translation = this->translation*translationTemp;
		this->rotation = this->rotation*rotationTemp;
		this->scale = this->scale*rotationTemp;
		for(list<CObject*>::iterator it = lstChilds.begin(); it != lstChilds.end(); ++it) {
			CObject* o = *it;
			o->move( tX,  tY,  tZ,  rX,  rY,  rZ,  sX,  sY, sZ);
		}
	}
	HRESULT initializeMesh()
	{
		if( FAILED( D3DXLoadMeshFromX( fileName, D3DXMESH_SYSTEMMEM,
									this->engine->d3ddev, NULL,
                                   &pD3DXMtrlBuffer, NULL, &dwNumMaterials,
                                   &pMesh ) ) )
		{
       
            MessageBox( NULL, L"No se puede encontrar el archivo", L"Meshes.exe", MB_OK );
            return E_FAIL;
        
		 }
		// We need to extract the material properties and texture names from the 
		// pD3DXMtrlBuffer
		D3DXMATERIAL* d3dxMaterials = ( D3DXMATERIAL* )pD3DXMtrlBuffer->GetBufferPointer();
		pMeshMaterials = new D3DMATERIAL9[dwNumMaterials];
		if( pMeshMaterials == NULL )
			return E_OUTOFMEMORY;
		pMeshTextures = new LPDIRECT3DTEXTURE9[dwNumMaterials];
		if( pMeshTextures == NULL )
			return E_OUTOFMEMORY;

		    for( DWORD i = 0; i < dwNumMaterials; i++ )
    {
        // Copy the material
        pMeshMaterials[i] = d3dxMaterials[i].MatD3D;

        // Set the ambient color for the material (D3DX does not do this)
        pMeshMaterials[i].Ambient = pMeshMaterials[i].Diffuse;

        pMeshTextures[i] = NULL;
        if( d3dxMaterials[i].pTextureFilename != NULL &&
            lstrlenA( d3dxMaterials[i].pTextureFilename ) > 0 )
        {
            // Create the texture
            if( FAILED( D3DXCreateTextureFromFileA( this->engine->d3ddev,
                                                    d3dxMaterials[i].pTextureFilename,
                                                    &pMeshTextures[i] ) ) )
            {
                // If texture is not in current folder, try parent folder
                const CHAR* strPrefix = "..\\";
                CHAR strTexture[MAX_PATH];
                strcpy_s( strTexture, MAX_PATH, strPrefix );
                strcat_s( strTexture, MAX_PATH, d3dxMaterials[i].pTextureFilename );
                // If texture is not in current folder, try parent folder
                if( FAILED( D3DXCreateTextureFromFileA( this->engine->d3ddev,
                                                        strTexture,
                                                        &pMeshTextures[i] ) ) )
                {
                    MessageBox( NULL, L"Could not find texture map", L"Meshes.exe", MB_OK );
                }
            }
        }
    }

    // Done with the material buffer
    pD3DXMtrlBuffer->Release();

    return S_OK;



	}
		

};

class Params
{
	
	public:
				//current ID
		int ID;
		//All the objects (tree)
		CObject* scene;
		//keep running graphics
		int notQuit;
		//We need the engine to run graphics
		CEngine* engine;

	Params()
	{
		ID = 0;
		scene = new CObject();
		notQuit = 1;
		engine = NULL;
		scene->pParent = NULL;
	}
	
};