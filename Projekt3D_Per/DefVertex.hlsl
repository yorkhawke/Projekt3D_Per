struct VS_IN
{
	float3 Pos : POSITION;

};

struct VS_OUT
{
	float4 Pos : SV_POSITION;
};

VS_OUT main(VS_IN input)
{
	VS_OUT output;

	output.Pos = float4(input.Pos, 1.0f);

	return output;
}
