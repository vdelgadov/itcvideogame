#ifndef OBJECT_DEF
#define OBJECT_DEF
#include <vector>
#include <list>
#include "Physics.h"
#include "Engine.h"
#include "Vehicle3D.h"
#include "Vertex.h"
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
	
	//Bounding Sphere needed by others
	D3DXVECTOR3 Center;
	float Radius;
	
	//
	double fPosX;
	double fPosY;
	double fPosZ;
	
	double fRotX;
	double fRotY;
	double fRotZ;
	
	double fScale;
	
	Vehicle3D vehicle;

	//Child list and parent
	list<CObject*> lstChilds;
	
private:
	//DECLARACIONES Y HANDLERS PARA SHADERS
	D3DXMATRIX mSceneWorld; //Matriz para calcular el mundo
	D3DXMATRIX mSceneWorldInv; //Matriz Inversa para calcular el mundo

	IDirect3DTexture9* mWhiteTex; //Textura en blanco para cuando no existen, no truenen los shaders
	ID3DXEffect* mFX; //EFECTO
	D3DXHANDLE   mhTech; //Handler de la tecnica
	D3DXHANDLE   mhWVP; //Handler World View Projection
	D3DXHANDLE   mhWorldInv; //Handler Matriz World Inverse
	D3DXHANDLE   mhEyePosW; //Handler de la posici�n del ojo
	D3DXHANDLE   mhTex; //Handler de la textura
	D3DXHANDLE   mhMtrl; //Handler del material
	D3DXHANDLE   mhLight; //Handler de TODAS las luces
	D3DXHANDLE   mhNormalMap; //Handler para el mapa normal
	DirLight mLight;	//Struct para todos  los tipos de luces
	LPDIRECT3DTEXTURE9* pMeshNormalMapTextures; //Apuntador a arreglo donde se almacenan los normal maps.
	std::vector<Mtrl> mShaderMtrls;
	ID3DXBuffer* adjBuffer;
	//FIN DE DECLARACIONES Y HANDLERS PARA SHADERS
public:	
	CObject()
	{
		this->fPosX = 0.0;
		this->fPosY = 0.0;
		this->fPosZ = 0.0;

		this->fRotX = 0.0;
		this->fRotY = 0.0;
		this->fPosZ = 0.0;
		
		this->fScale = 1.0;
		this->vehicle = Vehicle3D(1,Vector3D(0.0,0.0,0.0));
		//id
		this->ID = 0;
		D3DXMatrixTranslation(&translation,0.0,0.0,0.0);
		D3DXMatrixRotationYawPitchRoll(&rotation, 0.0, 0.0, 0.0);
		D3DXMatrixScaling(&scale, 1.0, 1.0, 1.0);

		//file
		this->fileName = NULL;
		//parent
		this->pParent = NULL;
	}
	CObject(int ID,float fPosX, float fPosY, float fPosZ, float fRotX, float fRotY, float fRotZ,float fScale, LPCTSTR fileName, CObject* parent,CEngine* engine)
	{
		//quitar cambiar por vehiculo
		this->fPosX = fPosX;
		this->fPosY = fPosY;
		this->fPosZ = fPosZ;

		this->fRotX = fRotX;
		this->fRotY = fRotY;
		this->fPosZ = fRotZ;
		
		this->fScale = fScale;
		//this->vehicle = Vehicle3D(1,Vector3D(fPosX,fPosY,fPosZ));
		//id
		this->ID = ID;

		D3DXMatrixTranslation(&translation,fPosX,fPosY,fPosZ);
		D3DXMatrixScaling(&scale, fScale, fScale, fScale);
		D3DXMatrixRotationYawPitchRoll(&rotation, fRotX, fRotY, fRotZ);

		//file
		this->fileName = fileName;
		//parent
		this->pParent = parent;
		this->engine = engine;
		this->initializeMesh();
		this->initializeWorldCoordinates();		
		this->initializeBoundingSphere();
		this->initializeShaders();
		printf ( "Centerx %f,Centery %f,Centerz %f, Radius%f\n", Center.x,Center.y,Center.z,Radius) ;

	}
	virtual void initializeWorldCoordinates()
	{
		CObject* o = this;
		while(o != NULL)
		{
			Vector3D pos = Vector3D(o->fPosX,o->fPosY,o->fPosZ);
			pos+=this->vehicle.getPos();
			this->vehicle.setPos( pos  );
			o = o->pParent;
		}
	}
	void initializeBoundingSphere()
	{
		LPDIRECT3DVERTEXBUFFER9 VertexBuffer;
		D3DXVECTOR3* Vertices;
		DWORD FVFVertexSize = D3DXGetFVFVertexSize(pMesh->GetFVF());
		pMesh->GetVertexBuffer(&VertexBuffer);
		VertexBuffer->Lock(0,0,(VOID**) &Vertices,D3DLOCK_DISCARD);
		D3DXComputeBoundingSphere(Vertices, pMesh->GetNumVertices(),FVFVertexSize,&Center, &Radius);
		VertexBuffer->Unlock();
		this->Radius = this->Radius*(float)this->fScale;
		this->Center = this->Center*(float)this->fScale;

		
	}
	
	virtual ~CObject() {
		for(list<CObject*>::iterator it = lstChilds.begin(); it != lstChilds.end(); ++it) {
			delete *it;
		}
	}
	virtual void render()
	{
		engine->d3ddev->SetTransform(D3DTS_WORLD,&( (this->scale) * (this->rotation) * (this->translation)) ); //

		mFX->SetValue(mhLight, &mLight, sizeof(DirLight));
		mFX->SetMatrix(mhWVP, &(mSceneWorld*engine->matProjection));
		mFX->SetValue(mhEyePosW, &engine->matView, sizeof(D3DXVECTOR3));
	
		UINT numPasses = 0;
		mFX->Begin(&numPasses, 0);
		mFX->BeginPass(0);

        for( DWORD i = 0; i < dwNumMaterials; i++ )
        {		
            //engine->d3ddev->SetMaterial( &pMeshMaterials[i] );
            //engine->d3ddev->SetTexture( 0, pMeshTextures[i] );
			mFX->SetValue(mhMtrl, &this->mShaderMtrls[i], sizeof(Mtrl));
	
			// If there is a texture, then use.
			if(this->pMeshTextures[i] != 0)
			{
				mFX->SetTexture(mhTex, this->pMeshTextures[i]);
			}
			else
			{
				mFX->SetTexture(mhTex, mWhiteTex);
			}
	
			mFX->SetTexture(mhNormalMap, this->pMeshNormalMapTextures[i]);

			mFX->CommitChanges();
            // Draw the mesh subset
            pMesh->DrawSubset( i );
        }
		mFX->EndPass();
		mFX->End();
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
				CObject* o = (*it)->find(id);
				if (o)
				{
					return o;
				}
			}
		}
		return NULL;
	}
	void move(float tX, float tY, float tZ, float rX, float rY, float rZ, float scale)
	{
		D3DXMATRIX translationTemp, rotationTemp, scaleTemp;
		
		D3DXMatrixTranslation(&translationTemp,tX,tY,tZ);
		D3DXMatrixRotationYawPitchRoll(&rotationTemp, rX, rY, rZ);
		D3DXMatrixScaling(&scaleTemp, scale, scale, scale);
		
		//if(!Physics::checkBoundingSphere(this, this->pParent)) // if no intersections change pos
		//{
			this->translation = this->translation*translationTemp;
			//Update world and relative coordinates
			Vector3D pos = Vector3D(this->fPosX,this->fPosY,this->fPosZ);
			pos+=this->vehicle.getPos();
			this->vehicle.setPos( pos  );
			this->fPosX += tX;
			this->fPosY += tY;
			this->fPosZ += tZ;
			

			this->rotation = this->rotation*rotationTemp;
			this->scale = this->scale*rotationTemp;
			for(list<CObject*>::iterator it = lstChilds.begin(); it != lstChilds.end(); ++it) {
				CObject* o = *it;
				o->moveNoChecks( tX,  tY,  tZ,  rX,  rY,  rZ,  scale);
			}
		//}
	}
	void moveNoChecks(float tX, float tY, float tZ, float rX, float rY, float rZ, float scale)
	{
		D3DXMATRIX translationTemp, rotationTemp, scaleTemp;
		
		D3DXMatrixTranslation(&translationTemp,tX,tY,tZ);
		D3DXMatrixRotationYawPitchRoll(&rotationTemp, rX, rY, rZ);
		D3DXMatrixScaling(&scaleTemp, scale, scale, scale);
		
		
		this->translation = this->translation*translationTemp;
		//Update world and relative coordinates
		Vector3D pos = Vector3D(this->fPosX,this->fPosY,this->fPosZ);
		pos+=this->vehicle.getPos();
		this->vehicle.setPos( pos  );
		this->fPosX += tX;
		this->fPosY += tY;
		this->fPosZ += tZ;
		

		this->rotation = this->rotation*rotationTemp;
		this->scale = this->scale*rotationTemp;
		for(list<CObject*>::iterator it = lstChilds.begin(); it != lstChilds.end(); ++it) {
			CObject* o = *it;
			o->moveNoChecks( tX,  tY,  tZ,  rX,  rY,  rZ,  scale);
		}
		
	}
	HRESULT initializeMesh()
	{

		if( FAILED( D3DXLoadMeshFromX( fileName, D3DXMESH_SYSTEMMEM,
									this->engine->d3ddev, &adjBuffer,
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

		pMeshNormalMapTextures = new LPDIRECT3DTEXTURE9[dwNumMaterials];
		if( pMeshNormalMapTextures == NULL )
			return E_OUTOFMEMORY;

		for( DWORD i = 0; i < dwNumMaterials; i++ )
		{
			
			Mtrl m;
			m.ambient   = d3dxMaterials[i].MatD3D.Diffuse;
			m.diffuse   = d3dxMaterials[i].MatD3D.Diffuse;
			m.spec      = d3dxMaterials[i].MatD3D.Specular;
			m.specPower = d3dxMaterials[i].MatD3D.Power;
			this->mShaderMtrls.push_back( m );
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
			//SHADERS
			// El prefijo del archivo para el normal map debe incluir nmap_, asimismo su extensi�n debe ser la misma que su textura.
			// Create the normal map texture
			const CHAR* strNormalMapPrefix = "nmap_";
			CHAR strNormalMapTexture[MAX_PATH];
			strcpy_s( strNormalMapTexture, MAX_PATH, strNormalMapPrefix );
			strcat_s( strNormalMapTexture, MAX_PATH, d3dxMaterials[i].pTextureFilename );

				if( FAILED( D3DXCreateTextureFromFileA( this->engine->d3ddev,
														strNormalMapTexture,
														&pMeshNormalMapTextures[i] ) ) )
				{
					// If texture is not in current folder, try parent folder
					const CHAR* strPrefixNM = "..\\";
					CHAR strTextureNM[MAX_PATH];
					strcpy_s( strTextureNM, MAX_PATH, strPrefixNM );
					strcat_s( strTextureNM, MAX_PATH, strNormalMapTexture );
					// If texture is not in current folder, try parent folder
					if( FAILED( D3DXCreateTextureFromFileA( this->engine->d3ddev,
															strTextureNM,
															&pMeshNormalMapTextures[i] ) ) )
					{
						MessageBox( NULL, L"Could not find Normal Texture map", L"Meshes.exe", MB_OK );
					}
				}
			}
		}

		// Done with the material buffer
		pD3DXMtrlBuffer->Release();

		return S_OK;
	}

	//Inicializa las declaraciones necesarias para crear shaders
	HRESULT initializeShaders(){
		

		InitAllVertexDeclarations(this->engine);
	
		mLight.ambient = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f);
		mLight.diffuse = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
		mLight.spec    = D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f);

		ID3DXMesh* tempMesh = 0;		
		tempMesh=this->pMesh;

		// Get the vertex declaration for the NMapVertex.
		D3DVERTEXELEMENT9 elems[MAX_FVF_DECL_SIZE];
		UINT numElems = 0;
		NMapVertex::Decl->GetDeclaration(elems, &numElems);

		// Clone the mesh to the NMapVertex format.
		ID3DXMesh* clonedTempMesh = 0;
		tempMesh->CloneMesh(D3DXMESH_MANAGED, elems, engine->d3ddev, &clonedTempMesh);

		// Now use D3DXComputeTangentFrameEx to build the TNB-basis for each vertex
		// in the mesh.  
		ReleaseCOM(tempMesh);
		D3DXComputeTangentFrameEx(
		  clonedTempMesh, // Input mesh
		  D3DDECLUSAGE_TEXCOORD, 0, // Vertex element of input tex-coords.  
		  D3DDECLUSAGE_BINORMAL, 0, // Vertex element to output binormal.
		  D3DDECLUSAGE_TANGENT, 0,  // Vertex element to output tangent.
		  D3DDECLUSAGE_NORMAL, 0,   // Vertex element to output normal.
		  0, // Options
		  0, // Adjacency
		  0.01f, 0.25f, 0.01f, // Thresholds for handling errors
		  &tempMesh, // Output mesh
		  0);         // Vertex Remapping

		ReleaseCOM(clonedTempMesh);
		
		tempMesh->Optimize(D3DXMESH_MANAGED | 
		D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE, 
		(DWORD*)adjBuffer->GetBufferPointer(), 0, 0, 0, &pMesh);
		
		ReleaseCOM(adjBuffer); // Done with buffer.
		ReleaseCOM(tempMesh);

		D3DXMatrixIdentity(&mSceneWorld);
		D3DXMatrixIdentity(&mSceneWorldInv);

		buildFX();
		return S_OK;
			
	}

	void buildFX(){
		// Create the FX from a .fx file.
		ID3DXBuffer* errors = 0;
		D3DXCreateEffectFromFile(engine->d3ddev, L"NormalMap.fx", 
			0, 0, D3DXSHADER_DEBUG, 0, &mFX, &errors);
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

		// Set parameters that do not vary:

		// World is the identity, so inverse is also identity.
		mFX->SetMatrix(mhWorldInv, &mSceneWorldInv);
		mFX->SetTechnique(mhTech);
	}

};
#endif