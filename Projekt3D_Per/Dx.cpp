#include "Dx.h"

Dx::Dx()
{

}

Dx::~Dx()
{
	depthStencilBuffer->Release();
    //depthStencilState->Release();
	depthStencilView->Release();
	rasterState->Release();
	ShaderDepth->Release();
	//swapChain->Release();

}

void Dx::init(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	HRESULT hr;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResDesc;
	D3D11_RASTERIZER_DESC rasterDesc;
	D3D11_VIEWPORT ViewPort;


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

		rasterDesc.AntialiasedLineEnable = false;
		rasterDesc.CullMode = D3D11_CULL_NONE;
		rasterDesc.DepthBias = 0;
		rasterDesc.DepthBiasClamp = 0.0f;
		rasterDesc.DepthClipEnable = true;
		rasterDesc.FillMode = D3D11_FILL_SOLID; // wireframe
		rasterDesc.FrontCounterClockwise = false;
		rasterDesc.MultisampleEnable = false;
		rasterDesc.ScissorEnable = false;
		rasterDesc.SlopeScaledDepthBias = 0.0f;

		device->CreateRasterizerState(&rasterDesc, &rasterState);

		deviceContext->RSSetState(rasterState);

		//ViewPort
		ViewPort.Width = (float)ScreenWidth;
		ViewPort.Height = (float)ScreenHeight;
		ViewPort.MinDepth = 0.0f;
		ViewPort.MaxDepth = 1.0f;
		ViewPort.TopLeftX = 0.0f;
		ViewPort.TopLeftY = 0.0f;
		deviceContext->RSSetViewports(1, &ViewPort);

	}


void Dx::endDX()
{

}

ID3D11DepthStencilState* Dx::getDepthState()
{
	return depthStencilState;
}

ID3D11DepthStencilView* Dx::getDepthView(ID3D11DeviceContext* devC)
{
	devC->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D10_CLEAR_STENCIL, 1.0f, 0);
	return depthStencilView;
}



