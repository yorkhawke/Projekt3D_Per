Texture2D Normal:register(t0);
Texture2D DiffuseA:register(t1);//Textures
Texture2D SpecA:register(t2);
Texture2D Position:register(t3);
Texture2D Occlusion:register(t4);
Texture2D ShadowMap:register(t5);

SamplerState pointSampler;

cbuffer Sun : register (b0)//lägg till ljusets matrix
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float3 SunPosition;
	float mp;
};

float4 main(in float4 screenPos : SV_Position) : SV_TARGET // Fixa shadow mapping någonsatan
{
	int3 sampleIndices = int3(screenPos.xy, 0);

	//float4 Spec = SpecA.Load(sampleIndices);
	float4 Pos = Position.Load(sampleIndices);
	float4 materialAmbient = float4(0.2f, 0.2f, 0.2f, 0.0f);
	float matSpec = 0.5;
	float4 A = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 Text = DiffuseA.Load(sampleIndices);
	
	float4 finalCol = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 normal = Normal.Load(sampleIndices);
	float Occ = Occlusion.Load(sampleIndices); 
	//float4 shadow = ShadowMap.Load(screenPos);
	float4 shadow = ShadowMap.Sample(pointSampler,screenPos.xy);

	//return float4(shadow.z, shadow.z, shadow.z, 1.0f);

	//return float4(shadow.x, shadow.y, shadow.z, 1.0f);
	float4 D = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 S = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float3 lightVector = normalize(SunPosition);

	A = materialAmbient*Ambient;
	
	float3 toEye = normalize(screenPos.xyz - Pos.xyz);

	float diffFac = dot(lightVector, normal);
	[flatten]
	if (diffFac > 0.0f)
	{
		float3 v = reflect(-lightVector, normal);
		float specFactor = pow(max(dot(v, toEye),0.0f), matSpec);
		
		D = diffFac*matSpec*Diffuse;
		S = specFactor*matSpec*Specular;
	}

	finalCol = Text*(A*Occ + (/*shadow**/D)) + S/**shadow*/;

	return finalCol;
}

