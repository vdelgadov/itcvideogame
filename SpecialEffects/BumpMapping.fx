float4x4 matWorldViewProj;
float4x4 matWorld;
float4 vecLightDir;
float4 vecEye;
texture ColorMap;
texture BumpMap;

struct VS_INPUT 
{
   float4 Position : POSITION0;
   float2 Tex : TEXCOORD;
   float3 Normal : NORMAL;
   float3 Tangent : TEXCOORD3;
   
};

struct VS_OUTPUT
{
   float4 Position : POSITION0;
   float2 Tex :TEXCOORD;
   float3 Light : TEXCOORD1;
   float3 View : TEXCOORD2;
};

VS_OUTPUT vs_main( VS_INPUT Input )
{
   VS_OUTPUT Out= (VS_OUTPUT)0;
   Out.Position = mul(Input.Position,matWorldViewProj);
   float3x3 worldToTangentSpace;
   worldToTangentSpace[0]=mul(Input.Tangent,matWorld);
   worldToTangentSpace[1]=mul(cross(Input.Tangent,Input.Normal),matWorld);
   worldToTangentSpace[2]=mul(Input.Normal,matWorld);
   
   Out.Tex=Input.Tex;
   
   float4 PosWorld = mul(Input.Position,matWorld);
   
   Out.Light = mul(worldToTangentSpace,vecLightDir);
   Out.View = mul(worldToTangentSpace,vecEye - PosWorld);
   
   return( Out );
}


sampler ColorMapSampler= sampler_state
{
Texture = <ColorMap>;
MinFilter = Linear;
MagFilter = Linear;
MipFilter = Linear;
AddressU = Clamp;
AddressV = Clamp;
};

sampler BumpMapSampler= sampler_state
{
Texture = <BumpMap>;
MinFilter = Linear;
MagFilter = Linear;
MipFilter = Linear;
AddressU = Clamp;
AddressV = Clamp;
};

float4 ps_main(float2 Tex: TEXCOORD0, float3 Light: TEXCOORD1,  float3 View: TEXCOORD2) : COLOR
{   

float4 Color = tex2D(ColorMapSampler, Tex);
float3 Normal = (2*(tex2D(BumpMapSampler, Tex)))-1.0;
float3 LightDir = normalize(Light);
float3 ViewDir = normalize(View);
float Diffuse = saturate(dot(Normal, LightDir));
float3 Reflect = normalize(2*Diffuse*Normal-LightDir);
//float Specular = min(pow(saturate(dot(Reflect,ViewDir)),3),Color.w);

float3 Half = normalize(LightDir + normalize(View));
float Diff = saturate(dot(Normal,LightDir));


//float3 Reflect = normalize(2 * Diff * Normal - LightDir);
float Specular = pow(saturate(dot(Normal, Half)), 25);
return 0.2 * Color + Color * Diffuse + Specular;   
}

technique RenderScene
{
    pass P0
    {          
        VertexShader = compile vs_2_0 vs_main();
        PixelShader  = compile ps_2_0 ps_main();
    }
}