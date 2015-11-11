cbuffer Matrixes : register(b0)
{
	float4x4 World;
	float4x4 View;
	float4x4 Proj;
}

struct VS_IN
{
	float3 Pos : POSITION;
};

float4 main(VS_IN input) :SV_POSITION
{
	float4 vPos = mul(World,input.Pos);
	vPos = mul(vPos, View);
	vPos = mul(vPos, Proj);

	return vPos;
}