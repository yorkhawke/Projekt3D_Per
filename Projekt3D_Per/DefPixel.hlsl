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

cbuffer SunMatrix : register (b3)
{
	float4x4 SW;
	float4x4 SV;
	float4x4 SP;
}

float4 main(in float4 screenPos : SV_Position) : SV_TARGET 
{
	int3 sampleIndices = int3(screenPos.xy, 0);

	float4 Pos = Position.Load(sampleIndices);
	float4 materialAmbient = float4(0.2f, 0.2f, 0.2f, 0.0f);
	float matSpec = 0.5;
	float4 A = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 Text = DiffuseA.Load(sampleIndices);
	
	float4 finalCol = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 normal = Normal.Load(sampleIndices);
	float Occ = Occlusion.Load(sampleIndices); 

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


	float4 ShadowPH = mul(mul(Pos, SV),SP);
	
	ShadowPH.xyz /= ShadowPH.w;
	//to the fucking SUN

	ShadowPH.x = ShadowPH.x / 2.0f + 0.5f;
	ShadowPH.y = ShadowPH.y / (-2.0f) + 0.5f;

	float d = ShadowMap.Sample(pointSampler, ShadowPH.xy).r;
	float bias = 0.0005f;
	float shade = 1.0f;
	if (d + bias < ShadowPH.z)
		shade = 0.5f;


	finalCol = Text*(A*Occ + (shade*D)) + S*shade;

	return finalCol;
}

