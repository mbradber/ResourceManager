

cbuffer cbPerObject
{
	float4x4 worldMatrix;
	float4x4 wvpMatrix;
};

// a struct for the vertex shader return value
struct VSOut
{
	float4 PosH : SV_POSITION;
    float4 Col : COLOR;
    
};

// the vertex shader
VSOut VS(float4 Pos : POSITION, float4 Col : COLOR)
{
    VSOut Output;

    Output.PosH = mul(Pos, wvpMatrix);
    Output.Col = Col;

    return Output;
}

// the pixel shader
float4 PS(VSOut pIn) : SV_TARGET
{
    return pIn.Col;
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
