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

struct VS_OUT
{
	float4 PosVS : SV_POSITION;
	float3 Nor : NORMAL;
	float2 Tex : TEXCOORD;
	float3 PosWS : POSITION;

};

VS_OUT main(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;

	output.PosVS = mul(float4(input.Pos, 1.0f), World);
	output.PosVS = mul(output.PosVS, View);
	output.PosVS = mul(output.PosVS, Proj);

	output.Nor = normalize(mul(float4(input.Nor, 1.0f), World)).xyz;

	output.PosWS = mul(float4(input.Pos,1.0f), World).xyz;

	output.Tex = input.Tex;
	return output;
}
