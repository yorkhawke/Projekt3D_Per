cbuffer Matrixes : register(b0)
{
	float4x4 World;
	float4x4 View;
	float4x4 Proj;
}

struct VS_IN
{
	float3 Pos : POSITION;
	float3 Nor : NORMAL;
	float2 Tex : TEXCOORD;
};

float4 main(VS_IN input) :SV_POSITION
{
	float4 vPos = mul(float4(input.Pos,1.0),World);
	vPos = mul(vPos, View);
	vPos = mul(vPos, Proj);

	return vPos;
}