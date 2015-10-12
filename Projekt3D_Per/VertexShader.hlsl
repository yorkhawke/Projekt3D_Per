cbuffer matrixes : register(b0)
{
	float4x4 World;
	float4x4 View;
	float4x4 Proj;
};

struct VS_IN
{
	float3 Pos : POSITION;
	float3 Nor: NORMAL;
	float2 Tex : TEXCOORD;
};

struct VS_OUT
{
	float4 Pos : SV_POSITION;
	float4 Nor : NORMAL;
	float2 Tex : TEXCOORD;
	float3 PosWS : POSITION;
};

VS_OUT main(VS_IN input)
{
	VS_OUT output;

	output.Pos = mul(float4(input.Pos, 1.0f),World);
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Proj);
	output.Nor = normalize(mul(input.Nor, World));

	output.Tex = input.Tex;
	output.PosWS = mul(input.Pos, World);

	return output;
}