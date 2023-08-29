//PixelShader.hlsl
//Constantes Globales para la Textura
Texture2D colorMap : register(t0);
Texture2D colorMap1 : register(t1);

SamplerState colorSampler : register(s0);

//Estructura de Entrada para el Shader
struct PS_Input
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD;
};

float4 PS_Main(PS_Input frag) : SV_TARGET
{
    float4 c_uno = colorMap.Sample(colorSampler, frag.uv);
    float4 c_dos = colorMap1.Sample(colorSampler, frag.uv);
    
    float4 colorFinal = c_uno * c_dos;

    return colorFinal;
}
