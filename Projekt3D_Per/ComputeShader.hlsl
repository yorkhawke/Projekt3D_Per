
Texture2D input : register (t4);//first ssao Second UAV
RWTexture2D<float> output : register (u0);//UAV blurred ssao

float gausCalc(float x)
{
	return exp(-sqrt(abs(x)) / 2);
}

#ifdef _FIRST
[numthreads(16, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{

	float GaNo = 0;
	float result = 0;

	//first run
	for (int i = -8; i < 8; i++)
	{
		GaNo += gausCalc(i);
	}

	for (int i = -8; i < 8; i++)
	{
		result += input[int2(DTid.x + i, DTid.y)] * (gausCalc(i) / GaNo);
	}
	output[DTid.xy] = result;
}
#else

[numthreads(1, 16, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
	float GaNo = 0;
	float result = 0;

	//first run
	for (int i = -8; i < 8; i++)
	{
		GaNo += gausCalc(i);
	}

	for (int i = -8; i < 8; i++)
	{
		result += input[int2(DTid.x, DTid.y+i)] * (gausCalc(i) / GaNo);
	}
	output[DTid.xy] = result;

}
#endif
//Prolly Works if i only use hardcoded weight cuz i think they are going to be the same allaround.... 