#include "GameSystem.h"

LRESULT CALLBACK WndProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(window, message, wParam, lParam);
	}

};

GameSystem::GameSystem()
{
	mainHwnd.setScreenHeight(ScreenHeight);
	mainHwnd.setScreenWidth(ScreenWidth);
	device = 0;
	swapChain = 0;
	deviceContext = 0;
}

GameSystem::~GameSystem()
{

}

HRESULT GameSystem::CreateSwapChain()
{
	DXGI_SWAP_CHAIN_DESC swapChainDesc;

	HRESULT hr;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferCount = 1;
	swapChainDesc.Windowed = TRUE;
	swapChainDesc.BufferDesc.Width = ScreenWidth;
	swapChainDesc.BufferDesc.Height = ScreenHeight;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.OutputWindow = mainHwnd.getHWND();
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.Flags = 0;

	hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_DEBUG, NULL, NULL, D3D11_SDK_VERSION, &swapChainDesc, &swapChain, &device, NULL, &deviceContext);
	return hr;
}

void GameSystem::StartGame(float gametime, float fps,HINSTANCE hinstance)
{


	//window
	mainHwnd.CreateHwnd(hinstance,WndProc);
	//time
	gameTime.Reset();
	gameTime.Update();	
	//Camera
	cam.SetProj(0.35*XM_PI, ScreenWidth / ScreenHeight, 0.5f, 20000.0f);
	cam.Update();
	// dx
	if (SUCCEEDED(CreateSwapChain()))
	{
		ID3D11Texture2D* pSwapChainBuffer = 0;
		swapChain->GetBuffer(0, _uuidof(ID3D11Texture2D), (void**)&pSwapChainBuffer);

		device->CreateRenderTargetView(pSwapChainBuffer, NULL, &Backbuffer);

		directX.init(device, deviceContext);

		pSwapChainBuffer->Release();
	}
	//create shaders
	createShaders();
	//Create Objects etc....
	setShaders();
	//store matrixes
	XMStoreFloat4x4(&matrix.World, XMMatrixTranspose(XMMatrixScaling(1.0, 1.0, 1.0)));
	XMStoreFloat4x4(&matrix.View, XMMatrixTranspose(cam.GetViewMa()));
	XMStoreFloat4x4(&matrix.Proj, XMMatrixTranspose(cam.GetProjMa()));
	//CreateBuffer
	CreateBuffers();

	hMap.CreateMap(300,300,300,300,device,deviceContext);
}

void GameSystem::CreateBuffers()
{
	//XMFLOAT3 p[4];
	//p[0].x = -1.0f;
	//p[0].z = 0.0f;
	//p[0].y = -1.0f;
	//p[1].x = -1.0f;
	//p[1].z = 0.0f;
	//p[1].y = 1.0f;
	//p[2].x = 1.0f;
	//p[2].z = 0.0f;
	//p[2].y = -1.0f;
	//p[3].x = 1.0f;
	//p[3].z = 0.0f;
	//p[3].y = 1.0f;

	//D3D11_BUFFER_DESC bd;
	//bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	//bd.ByteWidth = sizeof(XMFLOAT3)* 4;
	//bd.CPUAccessFlags = 0;
	//bd.Usage = D3D11_USAGE_DEFAULT;
	//bd.StructureByteStride = NULL;
	//bd.MiscFlags = NULL;

	//D3D11_SUBRESOURCE_DATA d;
	//ZeroMemory(&d, sizeof(d));
	//d.pSysMem = p;

	//device->CreateBuffer(&bd, &d, &ScreenBuffer);

	//WorldViewProj Matrixes
	D3D11_BUFFER_DESC WorMatri;
	WorMatri.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	WorMatri.ByteWidth = sizeof(XMFLOAT4X4)* 3;
	WorMatri.MiscFlags = 0;
	WorMatri.StructureByteStride = 0;
	WorMatri.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	WorMatri.Usage = D3D11_USAGE_DYNAMIC;

	D3D11_SUBRESOURCE_DATA DATA;

	ZeroMemory(&DATA, sizeof(DATA));
	DATA.pSysMem = &matrix;

	device->CreateBuffer(&WorMatri, &DATA, &MatrixBuffer);
}

void GameSystem::createShaders()
{
	HRESULT hr;

	//create VertexShader
	ID3DBlob* pVS = nullptr;
	hr = D3DCompileFromFile(L"VertexShader.hlsl", NULL, NULL, "main", "vs_5_0", 0, NULL, &pVS, NULL);

	hr = device->CreateVertexShader(pVS->GetBufferPointer(), pVS->GetBufferSize(), nullptr, &VertexShader);
	//create input layout to VertexShader
	D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	hr = device->CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), pVS->GetBufferPointer(), pVS->GetBufferSize(), &VertexLayout);
	pVS->Release();
	//create PixelShader
	ID3DBlob* pPS = nullptr;
	D3DCompileFromFile(L"PixelShader.hlsl", NULL, NULL, "main", "ps_5_0", NULL, NULL, &pPS, nullptr);

	hr = device->CreatePixelShader(pPS->GetBufferPointer(), pPS->GetBufferSize(), nullptr, &PixelShader);
	pPS->Release();
}

void GameSystem::setShaders()
{
	deviceContext->VSSetShader(VertexShader, nullptr, 0);
	deviceContext->PSSetShader(PixelShader, nullptr, 0);
	deviceContext->IASetInputLayout(VertexLayout);

}

void GameSystem::Render()
{

	
	gameTime.Update();
	gameTime.ShowFPS();

	float clearColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	deviceContext->ClearRenderTargetView(Backbuffer, clearColor);

	deviceContext->OMSetRenderTargets(1, &Backbuffer, directX.getDepthView(deviceContext));

	setShaders();

	cam.Update();
	//UPPDATING MATRIXBUFFER
	//Check for input
	cam.Input(gameTime.DeltaTime(), mainHwnd.getHWND());

	XMFLOAT3 Pos = cam.getPos();
	Pos.y = hMap.HMap(Pos.x, Pos.z) + 10.0f;
	cam.setPos(Pos);

	XMStoreFloat4x4(&matrix.View, XMMatrixTranspose(cam.GetViewMa()));
	XMStoreFloat4x4(&matrix.Proj, XMMatrixTranspose(cam.GetProjMa()));

	D3D11_MAPPED_SUBRESOURCE MapDATA;
	Matrix* temp;

	ZeroMemory(&MapDATA, sizeof(MapDATA));

	deviceContext->Map(MatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MapDATA);

	temp = (Matrix*)MapDATA.pData;
	temp->World = matrix.World;//change later depending on what object rendering? 
	temp->Proj = matrix.Proj;
	temp->View = matrix.View;

	deviceContext->Unmap(MatrixBuffer, 0);

	deviceContext->VSSetConstantBuffers(0, 1, &MatrixBuffer);

	hMap.render(deviceContext);

	//UINT duck;
	//UINT offset = 0;

	//duck = sizeof(XMFLOAT3);

	//deviceContext->IASetVertexBuffers(0, 1, &ScreenBuffer, &duck, &offset);

//	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

//	deviceContext->IASetInputLayout(VertexLayout);

	//FINISHED UPPDATING MATRIXBUFFER
	//deviceContext->Draw(4, 0);

	swapChain->Present(0, 0);

}

HWND GameSystem::getMainHwnd()
{
	return mainHwnd.getHWND();
}


