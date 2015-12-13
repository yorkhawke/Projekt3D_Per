cbuffer camPos : register(b2)
{
	float3 cam;
};

struct GS_IN
{
	float4 PosVS : SV_POSITION;
	float3 Nor : NORMAL;
	float2 Tex : TEXCOORD;
	float3 PosWS : POSITION;

};

struct GS_OUT
{
	float4 PosVS : SV_POSITION;
	float3 Nor : NORMAL;
	float2 Tex : TEXCOORD;
	float3 PosWS : POSITION;

};

[maxvertexcount(3)]
void main(
	triangle GS_IN input[3],
	inout TriangleStream< GS_OUT > outputStream
	)

{
	GS_OUT output = (GS_OUT)0;

	float3 v1 = input[0].PosWS - input[1].PosWS;
	float3 v2 = input[0].PosWS - input[2].PosWS;

	float3 normal = normalize(cross(v2, v1));

	if (dot(normalize((input[0].PosWS.xyz -cam)), normal) >= 0)
	{
		for (uint i = 0; i < 3; i++)
		{
			output.PosVS = input[i].PosVS;
			output.Nor = input[i].Nor;
			output.Tex = input[i].Tex;
			output.PosWS = input[i].PosWS;
			outputStream.Append(output);
		}
	}
}