
Texture2D input : register (t4);//first ssao Second UAV
RWTexture2D<float> output : register (u0);//UAV blurred ssao

//#if _FIRST
//[numthreads(16, 1, 1)]
//#else
//[numthreads(1, 16, 1)]
//#endif
//void main(uint3 DTid : SV_DispatchThreadID)
//{
//	
//	float GaNo;
//	float result=0;
//
//	//first run
//#if _FIRST
//	for (int i = -8; i < 8; i++)
//	{
//		GaNo = gcalc(DTid.x + i);
//	}
//
//	for (int i = -8; i < 8; i++)
//	{
//		result += input[DTid.x+i, DTid.y] * (gcalc(DTid.x + i) / GaNo);
//	}
//	output[DTid.xy] = result;
//#else
//	//Second run
//
//	for (int i = -8; i < 8; i++)
//	{
//		GaNo = gcalc(DTid.y + i);
//	}
//
//	for (int i = -8; i < 8; i++)
//	{
//		result += input[DTid.x, DTid.y + i] * (gcalc(DTid.y + i) / GaNo);
//	}
//#endif
//	output[DTid.xy] = result;
//}

float gausCalc(float x)
{
	return exp(-sqrt(x) / 2);
}

#if _FIRST
[numthreads(16, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{

	float GaNo;
	float result = 0;

	//first run
	for (int i = -8; i < 8; i++)
	{
		GaNo = gausCalc(DTid.x + i);
	}

	for (int i = -8; i < 8; i++)
	{
		result += input[int2(DTid.x + i, DTid.y)] * (gausCalc(DTid.x + i) / GaNo);
	}
	output[DTid.xy] = result;
	output[DTid.xy] = 0.0f;
	return;
}
#else

[numthreads(1, 16, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{

	float GaNo;
	float result = 0;
	for (int i = -8; i < 8; i++)
	{
		GaNo = gausCalc(DTid.y + i);
	}

	for (int i = -8; i < 8; i++)
	{
		result += input[int2(DTid.x, DTid.y + i)] * (gausCalc(DTid.y + i) / GaNo);
	}
	output[DTid.xy] = result;
	output[DTid.xy] = 0.0f;
	return;
}
#endif
//Prolly Works if i only use hardcoded weight cuz i think they are going to be the same allaround.... 