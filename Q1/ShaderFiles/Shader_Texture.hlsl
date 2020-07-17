cbuffer cbGameObjectInfo : register(b0)
{
    matrix gmtxWorld : packoffset(c0);
};
cbuffer cbCameraInfo : register(b1)
{
    matrix gmtxView : packoffset(c0);
    matrix gmtxProjection : packoffset(c4);
};

Texture2D t1 : register(t0);
SamplerState s1 : register(s0);

struct VS_INPUT
{
	float3 position : POSITION;
	float2 texCoord: TEXCOORD;
};

struct VS_OUTPUT
{
	float4 position: SV_POSITION;
	float2 texCoord: TEXCOORD;
};

VS_OUTPUT VSMain(VS_INPUT input)
{
	VS_OUTPUT output;
	output.position = mul(mul(mul(float4(input.position, 1.0f), gmtxWorld), gmtxView), gmtxProjection);
	output.texCoord = input.texCoord;
	return output;
}
float4 PSMain(VS_OUTPUT input) : SV_TARGET
{
	return t1.Sample(s1, input.texCoord);
}