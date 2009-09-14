float4x4 matViewProjection;
float4x4 matWorld;
float4 vecLightDir;
float4 vecEye;
float4 vDIC;
float4 vSpecIC;



struct VS_INPUT 
{
   float4 Position : POSITION0;
   float3 Normal : NORMAL;
   
};

struct VS_OUTPUT 
{
   float4 Position : POSITION0;
   float3 Light : TEXCOORD0;
   float3 Norm : TEXCOORD1;
   float3 View : TEXCOORD2;
   
};

VS_OUTPUT vs_main( VS_INPUT Input )
{
   VS_OUTPUT Output= (VS_OUTPUT)0;

   Output.Position = mul( Input.Position, matViewProjection );
   
   Output.Norm = mul(Input.Normal , matViewProjection );
   
   float4 PosWorld = mul(Input.Position, matWorld);
   
   Output.Light = vecLightDir;
   
   Output.View = vecEye - PosWorld;
   
   return Output;
   
   
   
   return( Output );
   
}


float4 ps_main(float3 Light: TEXCOORD0, float3 Norm : TEXCOORD1, float3 View : TEXCOORD2) : COLOR
{

float4 A = {0.3f, 0.0f, 0.0f, 1.0f};

float3 Normal = normalize(Norm);
float3 LightDir = normalize(Light);
float3 Half = normalize(LightDir + normalize(View));

float Diff = saturate(dot(Normal,LightDir));

//float3 Reflect = normalize(2 * Diff * Normal - LightDir);

float Specular = pow(saturate(dot(Normal, Half)), 25);

return A + vDIC * Diff + vSpecIC * Specular;

   //return(float4( 0.0f, 0.0f, 1.0f,1.0f ) );
   
}

technique RenderScene
{
    pass P0
    {          
        VertexShader = compile vs_2_0 vs_main();
        PixelShader  = compile ps_2_0 ps_main();
    }
}




