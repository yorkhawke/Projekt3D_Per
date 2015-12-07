Texture2D tex:register(t0);
SamplerState pointSampler:register(s1);

struct VS_OUT
{
	float4 Pos : SV_POSITION;
	float4 Nor : NORMAL;
	float2 Tex : TEXCOORD;
	float3 PosWS : POSITION;
};

float4 main(VS_OUT input) : SV_TARGET
{

	float4 Dir = float4(0, 1, 0, 0);

	float4 ambient = float4(0.2, 0.2, 0.2, 0);

	float4 finalCol = float4(0, 0, 0, 0);
	float4 diff = tex.Sample(pointSampler, input.Tex);

	finalCol += ambient;
	finalCol = finalCol *diff;

	finalCol += saturate(dot(normalize(Dir), input.Nor)*diff);
	return finalCol;
}

