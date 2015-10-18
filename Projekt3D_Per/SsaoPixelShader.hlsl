Texture2D Normal:register(t0);
Texture2D DiffuseA:register(t1);
Texture2D SpecA:register(t2);
Texture2D Position:register(t3);
Texture1D Rand:register(t4);

static const float3 gRandomSphereVectors[] = { // Random vectors inside unit sphere
	float3(0.53812504, 0.18565957, -0.43192000),
	float3(0.13790712, 0.24864247, 0.44301823),
	float3(0.33715037, 0.56794053, -0.00578950),
	float3(-0.69998050, -0.04511441, -0.00199656),
	float3(0.06896307, -0.15983082, -0.85477847),
	float3(0.05609944, 0.00695497, -0.18433520),
	float3(-0.01465364, 0.14027752, 0.07620370),
	float3(0.01001993, -0.19242250, -0.03444339),
	float3(-0.35775623, -0.53019690, -0.43581226),
	float3(-0.31692210, 0.10636073, 0.01586092),
	float3(0.01035035, -0.58698344, 0.00462939),
	float3(-0.08972908, -0.49408212, 0.32879040),
	float3(0.71199860, -0.01546900, -0.09183723),
	float3(-0.05338235, 0.05967581, -0.54118990),
	float3(0.03526766, -0.06318861, 0.54602677),
	float3(-0.47761092, 0.28479110, -0.02717160)
};

SamplerState pointSampler:register(s1);

//lights later
cbuffer proj
{
	float4x4 projview;
};

float main(in float4 screenPos : SV_Position) : SV_TARGET
{
	int3 sampleIndices = int3(screenPos.xy, 0);
	float4 Pos = Position.Load(sampleIndices);
	float4 normal = Normal.Load(sampleIndices);
	float occlusion = 0;
	float3 randomV = normalize(Rand.Sample(pointSampler, screenPos.x*screenPos.y).xyz);

		// ssao här fixa in deaptz buff och sedan bara googog  //gör ssao i egen run kolla på 

	for (int i = 0; i < 16; i++)
	{
		float3 ref = reflect(gRandomSphereVectors[i], randomV);
		float3 temp = Pos.xyz + sign(dot(ref, normal.xyz))*ref;

		float4 projected = mul(float4(temp, 1.0f), projview);
		temp = projected.xyz / projected.w;

		float2 texC;
		texC.x = (temp.x + 1.0)*0.5f;
		texC.y = (-temp.y + 1.0)*0.5f;

		float3 Occluder = Position.Sample(pointSampler, texC).xyz;
		float z = distance(Pos.xyz, Occluder);

		float distF = 1.0f - smoothstep(0.10f, 3.0f, z);
		float3 directionOclud = normalize(Occluder - Pos);
		occlusion += distF*max(dot(normal, directionOclud), 0);

	}
	occlusion /= 16.0f;
	float ambientOC = 1.0f - occlusion;
	
	return ambientOC;
}