struct Mtrl
{
	float4 ambient;
	float4 diffuse;
	float4 spec;
	float  specPower;
};

struct DirLight
{
	float4 ambient;
	float4 diffuse;
	float4 spec;
	float3 dirW;  
};

uniform extern float4x4 gW;
uniform extern float4x4 gVP;
uniform extern Mtrl     gMtrl;
uniform extern DirLight gLight;
uniform extern float3   gEyePosW;
uniform extern texture  gTex;

sampler TexS = sampler_state
{
	Texture = <gTex>;
	MinFilter = ANISOTROPIC;
	MaxAnisotropy = 8;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	AddressU  = WRAP;
    AddressV  = WRAP;
};
 
struct OutputVS
{
	float4 Position : POSITION0;
	float3 Light : TEXCOORD0;
	float3 Norm : TEXCOORD1;
	float3 View : TEXCOORD2;
	float2 tex0 : TEXCOORD3;
};

OutputVS BlinnPhongVS(float3 Position      : POSITION0,
                     float3 Normal   : NORMAL0,
                     float2 tex0      : TEXCOORD0)
{
    // Zero out our output.
	OutputVS outVS = (OutputVS)0;
	
	outVS.Position = mul( Input.Position, matViewProjection );
	outVS.Norm = mul(Input.Normal , matViewProjection );
	float4 PosWorld = mul(Input.Position, matWorld);
	outVS.Light = vecLightDir;
	outVS.View = vecEye - PosWorld;
	outVS.tex0 = tex0;
	
	// Done--return the output.
    return outVS;
}

float4 BlinnPhongPS(float3 Light: TEXCOORD0, 
					float3 Norm : TEXCOORD1, 
					float3 View : TEXCOORD2,
					float2 tex0 : TEXCOORD3,) : COLOR
{
	float3 Normal = normalize(Norm);
	float3 LightDir = normalize(Light);
	float3 ViewD  = normalize(View);

	float  fNDotL = dot( Normal, LightDir );
	float3 Reflection     = normalize( ( ( 2.0f * Normal ) * ( fNDotL ) ) - LightDir ); 
	float  fRDotV           = max( 0.0f, dot( Reflection, ViewD) );
	float4 BaseColor      = tex2D( TexS, tex0 );
	
	float4 Ambient=gMtrl.ambient*gLight.ambient;
	float4 Diffuse=gMtrl.ambient*gLight.ambient;
	float4 Specular=gMtrl.spec*gLight.spec;
	
	float4 fvTotalAmbient   = Ambient*fvBaseColor;
	float4 fvTotalDiffuse   = Diffuse * fNDotL * BaseColor; 
	float4 fvTotalSpecular  = Specular * pow( fRDotV, gMtrl.specPower );
   
   return( saturate( fvTotalAmbient + fvTotalDiffuse + fvTotalSpecular ) );
}

technique blinnPhongTech
{
    pass P0
    {
        vertexShader = compile vs_2_0 BlinnPhongVS();
        pixelShader  = compile ps_2_0 BlinnPhongPS();
    }
}