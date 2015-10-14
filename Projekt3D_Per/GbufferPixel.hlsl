Texture2D shaderTex;
SamplerState SampleType;

struct PS_IN
{
	float4 PosVS : SV_POSITION;
	float3 Nor : NORMAL;
	float2 Tex : TEXCOORD;
	float3 PosWS : POSITION;
};

struct PS_OUT
{
	float4 Nor : SV_TARGET0;
	float4 DiffuseA : SV_TARGET1;
	float4 SpecA : SV_TARGET2;
	float4 Pos : SV_TARGET3;
};

PS_OUT main(in PS_IN input)
{
	PS_OUT output;
	float3 SpecAlb = { 0, 0, 0 };
	float SpecPower = 0;

	float3 diffuseAlb = shaderTex.Sample(SampleType, input.Tex).xyz;
	float3 normal = normalize(input.Nor);

	output.Nor = float4(normal, 1.0f);
	output.DiffuseA = float4(diffuseAlb, 1.0f);
	output.SpecA = float4(SpecAlb, SpecPower);
	output.Pos = float4(input.PosWS, 1.0f);
	return output;
}

