

cbuffer cbPerObject
{
	float4x4 worldMatrix;
	float4x4 wvpMatrix;
};

//texture to sample
Texture2D diffuseMap;

SamplerState linearSample
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

// a struct for the vertex shader return value
struct VSOut
{
	float4 PosH : SV_POSITION;
    float2 texC : TEXCOORD;
};

// the vertex shader
VSOut VS(float4 Pos : POSITION, float2 Tex : TEXCOORD)
{
    VSOut Output;

    Output.PosH = mul(Pos, wvpMatrix);
    Output.texC = Tex;

    return Output;
}

// the pixel shader
float4 PS(VSOut pIn) : SV_TARGET
{
	float4 diffuse = diffuseMap.Sample(linearSample, pIn.texC);
    return diffuse;
}

// the primary technique
technique10 T0
{
    // the primary pass
    pass P0
    {
        SetVertexShader(CompileShader(vs_4_0, VS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, PS()));
    }
}
