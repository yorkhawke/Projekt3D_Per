Texture2D Normal:register(t0);
Texture2D DiffuseA:register(t1);
Texture2D SpecA:register(t2);
Texture2D Position:register(t3);
Texture2D Occlusion:register(t4);

SamplerState pointSampler;

float4 main(in float4 screenPos : SV_Position) : SV_TARGET
{
	int3 sampleIndices = int3(screenPos.xy, 0);

	float4 lightPos = float4(0, 0, 20, 0);
	float4 Pos = Position.Load(sampleIndices);
	float4 sun = float4(0, 100, 0, 0);
	float4 ambient = float4(0.5, 0.5, 0.5, 0);
	float4 Diff = DiffuseA.Load(sampleIndices);
	float4 Spec = SpecA.Load(sampleIndices);
	float4 finalCol = float4(0, 0, 0, 0);
	float4 normal = Normal.Load(sampleIndices);
	float Occ = Occlusion.Load(sampleIndices);
	
	//return normal;
	finalCol += ambient*Occ;
	finalCol = finalCol *Diff;

	finalCol += saturate(finalCol);
	return finalCol;

}

