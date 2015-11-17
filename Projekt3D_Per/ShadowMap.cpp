#include "ShadowMap.h"

ShadowMap::ShadowMap()
{

}

ShadowMap::~ShadowMap()
{

}

void ShadowMap::StartUp(ID3D11Device* device, XMFLOAT3 Sun,XMFLOAT4X4 lightView)
{
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResDesc;

	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
	depthBufferDesc.Width = ScreenWidth;
	depthBufferDesc.Height = ScreenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	//depth 
	device->CreateTexture2D(&depthBufferDesc, NULL, &depthStencilBuffer);

	D3D11_DEPTH_STENCIL_VIEW_DESC dpVdesc;

	dpVdesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dpVdesc.Flags = 0;
	dpVdesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dpVdesc.Texture2D.MipSlice = 0;

	device->CreateDepthStencilView(depthStencilBuffer, &dpVdesc, &depthStencilView);

	shaderResDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	shaderResDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResDesc.Texture2D.MostDetailedMip = 0;
	shaderResDesc.Texture2D.MipLevels = 1;

	device->CreateShaderResourceView(depthStencilBuffer, &shaderResDesc, &ShaderDepth);


	//Shader

	HRESULT hr;
	//create input layout to VertexShader
	D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	ID3DBlob* pSSVS = nullptr;
	D3DCompileFromFile(L"ShadowMap.hlsl", NULL, NULL, "main", "vs_5_0", 0, NULL, &pSSVS, NULL);

	hr = device->CreateVertexShader(pSSVS->GetBufferPointer(), pSSVS->GetBufferSize(), nullptr, &shadowMapVertexShader);

	hr = device->CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), pSSVS->GetBufferPointer(), pSSVS->GetBufferSize(), &Layout);

	//D3D11_BUFFER_DESC bd;
	//bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	//bd.ByteWidth = sizeof(XMFLOAT3);
	//bd.CPUAccessFlags = 0;
	//bd.Usage = D3D11_USAGE_DEFAULT;
	//bd.StructureByteStride = NULL;
	//bd.MiscFlags = NULL;

	//D3D11_SUBRESOURCE_DATA d;
	//ZeroMemory(&d, sizeof(d));
	//d.pSysMem = &Sun;

	//device->CreateBuffer(&bd, &d, &lightBuffer);
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
	bd.ByteWidth = sizeof(XMFLOAT3) * 4;
	bd.CPUAccessFlags = 0;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.StructureByteStride = NULL;
	bd.MiscFlags = NULL;

	D3D11_SUBRESOURCE_DATA d;
	ZeroMemory(&d, sizeof(d));
	d.pSysMem = p;
	device->CreateBuffer(&bd, &d, &lightBuffer);

	//WorldViewProj Matrixes
	D3D11_BUFFER_DESC WorMatri;
	WorMatri.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	WorMatri.ByteWidth = sizeof(Matrix);
	WorMatri.MiscFlags = 0;
	WorMatri.StructureByteStride = 0;
	WorMatri.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	WorMatri.Usage = D3D11_USAGE_DYNAMIC;

	D3D11_SUBRESOURCE_DATA DATA;

	shadowMatrix.View = lightView;
	XMStoreFloat4x4(&shadowMatrix.World, XMMatrixTranspose(XMMatrixScaling(1.0, 1.0, 1.0)));
	XMStoreFloat4x4(&shadowMatrix.Proj, XMMatrixTranspose(XMMatrixPerspectiveFovLH(XM_PI / 3, 1040.0f / 800.0f, 0.5, 20000.0f)));
	ZeroMemory(&DATA, sizeof(DATA));
	DATA.pSysMem = &shadowMatrix;

	device->CreateBuffer(&WorMatri, &DATA, &MatrixBuffer);

	
}

void ShadowMap::Render(ID3D11DeviceContext* devCon)
{
	devCon->VSSetShader(shadowMapVertexShader, nullptr,0);
	devCon->PSSetShader(nullptr, nullptr, 0);
	devCon->IASetInputLayout(Layout);

	devCon->ClearDepthStencilView(depthStencilView, 0, 0, 0);
	devCon->OMSetRenderTargets(0, NULL,depthStencilView);

	//RENDERTARGET SET 
	UINT duck;
	UINT offset = 0;

	duck = sizeof(XMFLOAT3);

	devCon->IASetVertexBuffers(0, 1, &lightBuffer, &duck, &offset);
	devCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	devCon->VSSetConstantBuffers(0, 1, &MatrixBuffer);
	//Drawcall
	devCon->Draw(4, 0);

	ID3D11RenderTargetView* temp = { NULL };
	devCon->OMSetRenderTargets(1, &temp, nullptr);
	devCon->PSSetShaderResources(5, 1, &ShaderDepth);
}

