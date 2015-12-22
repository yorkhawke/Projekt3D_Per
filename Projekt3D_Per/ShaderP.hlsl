
struct shadow
{
	float4 vPos : POSITION;
	float sDepth : TEXCOORD0;
};

float4 main(shadow IN) : SV_TARGET
{
	return float4(IN.sDepth, IN.sDepth, IN.sDepth, 1.0f);
}