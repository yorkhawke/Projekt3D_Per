#include "defered.h"

Defered::Defered()
{
	
}

Defered::~Defered()
{
	for (int i = 0; i < BufferC; i++)
	{
		renderTargetTex[i]->Release();
		ShaderResource[i]->Release();
		renderTargetViewA[i]->Release();
	}

}

void Defered::createBuffers(ID3D11DeviceContext* devCon, ID3D11Device* dev)
{

}

void Defered::StartUp(ID3D11Device* dev, ID3D11DeviceContext *devCon, IDXGISwapChain* swapChain)
{
	HRESULT hr;
	//----------------------------------RESOURCES-----------------------------------
	D3D11_TEXTURE2D_DESC textureDesc;
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResDesc;

	ZeroMemory(&textureDesc, sizeof(textureDesc));
	textureDesc.Width = ScreenWidth;// Texture width
	textureDesc.Height = ScreenHeight; // likewise
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	swapChain->GetBuffer(0, _uuidof(ID3D11Texture2D), (void**)&backB);

	dev->CreateRenderTargetView(backB, NULL, &BackBuff);

	for (int i = 0; i < BufferC; i++)
	{
		hr = dev->CreateTexture2D(&textureDesc, NULL, &renderTargetTex[i]);
	}

	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	shaderResDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	shaderResDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResDesc.Texture2D.MostDetailedMip = 0;
	shaderResDesc.Texture2D.MipLevels = 1;


	for (int i = 0; i < BufferC; i++)
	{

		hr = dev->CreateRenderTargetView(renderTargetTex[i], &renderTargetViewDesc, &renderTargetViewA[i]);
	}

	for (int i = 0; i < BufferC; i++)
	{
		hr = dev->CreateShaderResourceView(renderTargetTex[i], &shaderResDesc, &ShaderResource[i]);
	}
	//----------------------------------RESOURCES-----------------------------------

	//---------------SHADERS--------------------------
	//create GVertex shader 
	ID3DBlob* pGVS = nullptr;
	D3DCompileFromFile(L"GbufferVertex.hlsl", NULL, NULL, "main", "vs_5_0", 0, NULL, &pGVS, NULL);

	hr = dev->CreateVertexShader(pGVS->GetBufferPointer(), pGVS->GetBufferSize(), nullptr, &GVertexShader);

	//create input layout to GVertex shader
	D3D11_INPUT_ELEMENT_DESC inputDescG[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	hr = dev->CreateInputLayout(inputDescG, ARRAYSIZE(inputDescG), pGVS->GetBufferPointer(), pGVS->GetBufferSize(), &GVertexLayout);
	pGVS->Release();

	//create VertexShader
	ID3DBlob* pVS = nullptr;
	D3DCompileFromFile(L"DefVertex.hlsl", NULL, NULL, "main", "vs_5_0", 0, NULL, &pVS, NULL);

	hr = dev->CreateVertexShader(pVS->GetBufferPointer(), pVS->GetBufferSize(), nullptr, &DefVertexShader);


	//create input layout to VertexShader
	D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	hr = dev->CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), pVS->GetBufferPointer(), pVS->GetBufferSize(), &DefVertexLayout);
	pVS->Release();

	////create GPixel shader
	ID3DBlob* pPGS = nullptr;
	D3DCompileFromFile(L"GbufferPixel.hlsl", NULL, NULL, "main", "ps_5_0", NULL, NULL, &pPGS, nullptr);

	hr = dev->CreatePixelShader(pPGS->GetBufferPointer(), pPGS->GetBufferSize(), nullptr, &GPixelShader);
	pPGS->Release();

	//create PixelShader
	ID3DBlob* pPS = nullptr;
	D3DCompileFromFile(L"DefPixel.hlsl", NULL, NULL, "main", "ps_5_0", NULL, NULL, &pPS, nullptr);

	hr = dev->CreatePixelShader(pPS->GetBufferPointer(), pPS->GetBufferSize(), nullptr, &DefPixelShader);
	pPS->Release();
	//---------------SHADERS--------------------------

	//----------------------------BUFFER-------------------------------------
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

	dev->CreateBuffer(&bd, &d, &ScreenBuff);
	//----------------------------BUFFER-------------------------------------
}

//set rendertarget
void Defered::OMSetRender(ID3D11Device* dev, ID3D11DeviceContext *devCon, ID3D11DepthStencilView* dpStencilView)
{
	devCon->OMSetRenderTargets(BufferC, renderTargetViewA, dpStencilView);
}

//Buffer
void Defered::clearBuffer(ID3D11DeviceContext *devCon)
{
	float clearColor[] = { 0.0f, 0.0f, 0.0f, 1 };

	for (int i = 0; i < BufferC; i++)
	{
		devCon->ClearRenderTargetView(renderTargetViewA[i], clearColor);
	}
	devCon->ClearRenderTargetView(BackBuff, clearColor);
}

void Defered::createShaders(ID3D11Device* dev, ID3D11DeviceContext *devCon)
{

}

void Defered::nullRender(ID3D11DeviceContext* devCon)
{
	ID3D11RenderTargetView* temp[BufferC] = { NULL, NULL, NULL, NULL };
	devCon->OMSetRenderTargets(BufferC, temp, nullptr);
}

void Defered::setGBufferShaders(ID3D11DeviceContext* devCon)
{
	devCon->VSSetShader(GVertexShader, nullptr, 0);
	devCon->PSSetShader(GPixelShader, nullptr, 0);
	devCon->IASetInputLayout(GVertexLayout);
}

void Defered::setBackBufferShaders(ID3D11DeviceContext* devCon)
{
	devCon->VSSetShader(DefVertexShader, nullptr, 0);
	devCon->PSSetShader(DefPixelShader, nullptr, 0);
	devCon->IASetInputLayout(DefVertexLayout);
}

void Defered::OMSetBackBuff(ID3D11DeviceContext *devCon)
{
	devCon->OMSetRenderTargets(1, &BackBuff, nullptr);
}

void Defered::setShaderResources(ID3D11DeviceContext *devCon)
{

	devCon->PSSetShaderResources(0, 1, &ShaderResource[0]);//Normal
	devCon->PSSetShaderResources(1, 1, &ShaderResource[1]);//diffuseA
	devCon->PSSetShaderResources(2, 1, &ShaderResource[2]);//SpecA
	devCon->PSSetShaderResources(3, 1, &ShaderResource[3]);//Position

}

void Defered::test(ID3D11DeviceContext* devCon)
{
	devCon->OMSetRenderTargets(1, &renderTargetViewA[0], nullptr);
}


void Defered::Render(ID3D11Device* dev, ID3D11DeviceContext *devCon)
{
	UINT duck;
	UINT offset = 0;

	duck = sizeof(XMFLOAT3);

	devCon->IASetVertexBuffers(0, 1, &ScreenBuff, &duck, &offset);

	devCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	devCon->Draw(4, 0);
}

void Defered::CloseBuffers(ID3D11DeviceContext* devCon)
{
	ID3D11RenderTargetView* otemp = { NULL };
	devCon->OMSetRenderTargets(1, &otemp, nullptr);

	ID3D11ShaderResourceView* temp[BufferC+2] = { NULL };
	devCon->PSSetShaderResources(0, BufferC+2, temp);
}




