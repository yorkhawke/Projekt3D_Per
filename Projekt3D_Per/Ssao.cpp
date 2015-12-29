#include "SSao.h"

void SSao::randomTex(ID3D11Device* dev, ID3D11DeviceContext *devCon)
{
	
	XMFLOAT4 randValues[wind];

	std::default_random_engine gen;
	std::uniform_real_distribution<float> random(-1.0f, 1.0f);

	for (int i = 0; i < wind; i++)
	{
		randValues[i].x = random(gen);
		randValues[i].y = random(gen);
		randValues[i].z = random(gen);
		randValues[i].w = random(gen);
	}

	D3D11_SUBRESOURCE_DATA indata;
	indata.pSysMem = randValues;
	indata.SysMemPitch = wind * sizeof(XMFLOAT4);
	indata.SysMemSlicePitch = 0;

	D3D11_TEXTURE1D_DESC texDesc;
	texDesc.Width = wind;
	texDesc.MipLevels = 1;
	texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	texDesc.Usage = D3D11_USAGE_IMMUTABLE;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;
	texDesc.ArraySize = 1;

	ID3D11Texture1D* randomTex = 0;
	HRESULT hr;

	hr = dev->CreateTexture1D(&texDesc, &indata, &randomTex);

	D3D11_SHADER_RESOURCE_VIEW_DESC viewD;
	viewD.Format = texDesc.Format;
	viewD.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
	viewD.Texture1D.MipLevels = texDesc.MipLevels;
	viewD.Texture1D.MostDetailedMip = 0;

	hr = dev->CreateShaderResourceView(randomTex, &viewD, &ShaderRandTex);

	devCon->PSSetShaderResources(4, 1, &ShaderRandTex);

}


void SSao::startUp(ID3D11Device* dev, ID3D11DeviceContext *devCon)
{
	HRESULT hr;
	//create input layout to VertexShader
	D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	ID3DBlob* pSSVS = nullptr;
	D3DCompileFromFile(L"SSaoVertexShader.hlsl", NULL, NULL, "main", "vs_5_0", 0, NULL, &pSSVS, NULL);

	hr = dev->CreateVertexShader(pSSVS->GetBufferPointer(), pSSVS->GetBufferSize(), nullptr, &SSaoV);

	hr = dev->CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), pSSVS->GetBufferPointer(), pSSVS->GetBufferSize(), &Layout);

	pSSVS->Release();

	ID3DBlob* pSSPS = nullptr;
	D3DCompileFromFile(L"SSaoPixelShader.hlsl", NULL, NULL, "main", "ps_5_0", NULL, NULL, &pSSPS, nullptr);

	hr = dev->CreatePixelShader(pSSPS->GetBufferPointer(), pSSPS->GetBufferSize(), nullptr, &SSaoP);
	pSSPS->Release();

	D3D_SHADER_MACRO Gaus[3] = { {"_FIRST",NULL},{"_SECOND",NULL},{NULL,NULL} };

	const D3D_SHADER_MACRO first[] =
	{
		{"_FIRST","0"},
		{NULL,NULL }
	};

	//GAUSSIAN FILTER
	ID3DBlob* pCp = nullptr;
	D3DCompileFromFile(L"ComputeShader.hlsl", first,NULL, "main", "cs_5_0", NULL, NULL, &pCp, nullptr);
	hr = dev->CreateComputeShader(pCp->GetBufferPointer(), pCp->GetBufferSize(), nullptr, &SSaoCF);
	pCp->Release();//How to know if this works?

	ID3DBlob* pCp2 = nullptr;
	D3DCompileFromFile(L"ComputeShader.hlsl", NULL, NULL, "main", "cs_5_0", NULL, NULL, &pCp2, nullptr);
	hr = dev->CreateComputeShader(pCp2->GetBufferPointer(), pCp2->GetBufferSize(), nullptr, &SSaoCS);
	pCp2->Release();
	//should be its own class but noo time makes Per a bad boy!!!!
	D3D11_TEXTURE2D_DESC gausDesc;

	gausDesc.Width = ScreenWidth;
	gausDesc.Height = ScreenHeight;
	gausDesc.MipLevels = 1;
	gausDesc.ArraySize = 1;
	gausDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	gausDesc.SampleDesc.Count = 1;
	gausDesc.SampleDesc.Quality = 0;
	gausDesc.Usage = D3D11_USAGE_DEFAULT;
	gausDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	gausDesc.CPUAccessFlags = 0;
	gausDesc.MiscFlags = 0;

	ID3D11Texture2D* GausTex;
	hr = dev->CreateTexture2D(&gausDesc, 0, &GausTex);

	D3D11_SHADER_RESOURCE_VIEW_DESC gST;

	gST.Format = gausDesc.Format;
	gST.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	gST.Texture2D.MipLevels = 1;
	gST.Texture2D.MostDetailedMip = 0;
	
	hr = dev->CreateShaderResourceView(GausTex, &gST, &CShaderTex);

	D3D11_UNORDERED_ACCESS_VIEW_DESC UAVDesc;

	UAVDesc.Format = gausDesc.Format;
	UAVDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	UAVDesc.Texture2D.MipSlice = 0;

	dev->CreateUnorderedAccessView(GausTex, &UAVDesc, &blur);

	//GAUSSIAN FILTER

	
	//SSao pass

	// SamplerState
	D3D11_SAMPLER_DESC sd;

	sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.BorderColor[0] = 0;
	sd.BorderColor[1] = 0;
	sd.BorderColor[2] = 0;
	sd.BorderColor[3] = 0;
	sd.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sd.MaxAnisotropy = 8;
	sd.MaxLOD = D3D11_FLOAT32_MAX;
	sd.MinLOD = 0;
	sd.MipLODBias = 0;
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;

	dev->CreateSamplerState(&sd, &Pointsampler);

	D3D11_BUFFER_DESC pbb;
	ZeroMemory(&pbb, sizeof(D3D11_BUFFER_DESC));

	pbb.Usage = D3D11_USAGE_DYNAMIC;
	pbb.ByteWidth = sizeof(XMFLOAT4X4);
	pbb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	pbb.MiscFlags = 0;
	pbb.StructureByteStride = 0;
	pbb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	dev->CreateBuffer(&pbb, nullptr, &PixelBuffer);

	XMFLOAT3 p[4];

	p[0].x = -1.0f;
	p[0].z = 0.0f;
	p[0].y = -1.0f;
	p[1].x = -1.0f;
	p[1].z = 0.0f;
	p[1].y = 1.0f;
	p[2].x = 1.0f;
	p[2].z = 0.0f;
	p[2].y = -1.0f;
	p[3].x = 1.0f;
	p[3].z = 0.0f;
	p[3].y = 1.0f;

	D3D11_BUFFER_DESC bd;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.ByteWidth = sizeof(XMFLOAT3)* 4;
	bd.CPUAccessFlags = 0;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.StructureByteStride = NULL;
	bd.MiscFlags = NULL;

	D3D11_SUBRESOURCE_DATA d;
	ZeroMemory(&d, sizeof(d));
	d.pSysMem = p;

	dev->CreateBuffer(&bd, &d, &ScreenBuffer);

	D3D11_TEXTURE2D_DESC textureDesc;
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResDesc;

	ZeroMemory(&textureDesc, sizeof(textureDesc));
	textureDesc.Width = ScreenWidth;
	textureDesc.Height = ScreenHeight;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	hr = dev->CreateTexture2D(&textureDesc, NULL, &SSaoTex);

	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	shaderResDesc.Format = textureDesc.Format;
	shaderResDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResDesc.Texture2D.MostDetailedMip = 0;
	shaderResDesc.Texture2D.MipLevels = 1;


	hr = dev->CreateRenderTargetView(SSaoTex, &renderTargetViewDesc, &SSaoRTV);
	hr = dev->CreateShaderResourceView(SSaoTex, &shaderResDesc, &SSaoSRV);

}

void SSao::renderPass(ID3D11Device* dev, ID3D11DeviceContext *devCon, XMFLOAT4X4 projView)
{

	devCon->VSSetShader(SSaoV, nullptr, 0);
	devCon->PSSetShader(SSaoP, nullptr, 0);
	devCon->IASetInputLayout(Layout);

	//RENDERTARGET SET 

	D3D11_MAPPED_SUBRESOURCE MapDATA;

	devCon->Map(PixelBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MapDATA);

	memcpy(MapDATA.pData, &projView, sizeof(projView));

	devCon->Unmap(PixelBuffer, 0);

	devCon->PSSetConstantBuffers(0, 1, &PixelBuffer);

	devCon->PSSetSamplers(1, 1, &Pointsampler);

	float clearColor[] = { 0.0f, 0.0f, 0.0f, 1 };

	devCon->ClearRenderTargetView(SSaoRTV, clearColor);

	devCon->OMSetRenderTargets(1, &SSaoRTV, nullptr);

	randomTex(dev, devCon);

	UINT duck;
	UINT offset = 0;

	duck = sizeof(XMFLOAT3);

	devCon->IASetVertexBuffers(0, 1, &ScreenBuffer, &duck, &offset);
	devCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//Drawcall
	devCon->Draw(4, 0);

	ID3D11RenderTargetView* temp = { NULL };
	devCon->OMSetRenderTargets(1, &temp, nullptr);

	devCon->PSSetShaderResources(4, 1,&SSaoSRV);//fixa shadernbara
	////gauss run
	//devCon->CSSetShader(SSaoCF,nullptr,0);
	//devCon->CSSetUnorderedAccessViews(0,1,&blur,0);
	////First run
	//devCon->CSSetShaderResources(4, 1, &SSaoSRV);
	//devCon->Dispatch(32, 32, 1);
	////Second run and final 
	//devCon->CSSetShaderResources(4, 1, &CShaderTex);
	//devCon->CSSetShader(SSaoCS, nullptr, 0);
	//devCon->Dispatch(32, 32, 1);
	////Drunk and wonderfull
	//devCon->PSSetShaderResources(4, 1, &CShaderTex);//fixa shadernbara
}

