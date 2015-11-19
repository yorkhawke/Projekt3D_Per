#include "ShadowMap.h"

ShadowMap::ShadowMap()
{

}

ShadowMap::~ShadowMap()
{

}

void ShadowMap::StartUp(ID3D11Device* device, ID3D11DeviceContext* devCon,Matrix light)
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

	D3D11_RENDER_TARGET_VIEW_DESC testDesc;
	testDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	testDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	testDesc.Texture2D.MipSlice = 0;

	device->CreateRenderTargetView(depthStencilBuffer, &testDesc, &test);
	//Shader

	HRESULT hr;
	//create input layout to VertexShader
	D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	ID3DBlob* pSSVS = nullptr;
	D3DCompileFromFile(L"ShadowMap.hlsl", NULL, NULL, "main", "vs_5_0", 0, NULL, &pSSVS, NULL);

	hr = device->CreateVertexShader(pSSVS->GetBufferPointer(), pSSVS->GetBufferSize(), nullptr, &shadowMapVertexShader);


}

void ShadowMap::prepRun(ID3D11DeviceContext* devCon)
{
	devCon->VSSetShader(shadowMapVertexShader, nullptr,0);
	devCon->PSSetShader(nullptr, nullptr, 0);

	devCon->ClearDepthStencilView(depthStencilView, 0, 0, 0);
	ID3D11RenderTargetView* temp = { NULL };
	devCon->OMSetRenderTargets(1, &temp,depthStencilView);
	// Ljusmatrixen då asså och kolla ifall avståndet mellan shadowmappen och den pixeln ifall vilken som är störst....

}

void ShadowMap::close(ID3D11DeviceContext* devCon)
{
	ID3D11RenderTargetView* temp = { NULL };
	devCon->OMSetRenderTargets(1, &temp, nullptr);
	devCon->PSSetShaderResources(5, 1, &ShaderDepth);
}

void ShadowMap::testis(ID3D11DeviceContext* devCon)
{
	devCon->OMSetRenderTargets(1, &test, nullptr);
}


