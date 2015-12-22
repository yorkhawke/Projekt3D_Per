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

	float3 v1 = input[1].PosWS - input[0].PosWS;
	float3 v2 = input[2].PosWS - input[0].PosWS;

	float3 normal = normalize(cross(v1, v2));

	if (dot((input[0].PosWS - float4(cam, 1.0f)), normal) >= 0)
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