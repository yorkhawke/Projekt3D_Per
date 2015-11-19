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
	cam.SetProj(0.35*XM_PI, ScreenWidth / (ScreenHeight*1.0f), 0.5f, 20000.0f);
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

	//TEST
	//createShaders();

	//Defered

	DeferedRendering.StartUp(device, deviceContext,swapChain);

	//Create Objects etc....

	//TEST
	//setShaders();

	//store matrixes
	XMStoreFloat4x4(&matrix.World, XMMatrixTranspose(XMMatrixScaling(1.0, 1.0, 1.0)));
	XMStoreFloat4x4(&matrix.View, XMMatrixTranspose(cam.GetViewMa()));
	XMStoreFloat4x4(&matrix.Proj, XMMatrixTranspose(cam.GetProjMa()));
	//CreateBuffer
	CreateBuffers();

	hMap.CreateMap(300,300,300,300,device,deviceContext);

	obj.LoadObjFile(L"skull.obj");
	obj.createTexture(device, deviceContext, L"teapot.png");
	obj.createbuff(device);

	Ssao.startUp(device, deviceContext);

	//View projektion from sunlight
	shadow.StartUp(device,deviceContext,sunMatrix);
}

void GameSystem::CreateBuffers()
{

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
	sd.Filter = D3D11_FILTER_ANISOTROPIC;
	sd.MaxAnisotropy = 8;
	sd.MaxLOD = D3D11_FLOAT32_MAX;
	sd.MinLOD = 0;
	sd.MipLODBias = 0;

	device->CreateSamplerState(&sd, &SampleState);

	deviceContext->PSSetSamplers(0, 1, &SampleState);

	//WorldViewProj Matrixes
	D3D11_BUFFER_DESC WorMatri;
	WorMatri.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	WorMatri.ByteWidth = sizeof(Matrix);
	WorMatri.MiscFlags = 0;
	WorMatri.StructureByteStride = 0;
	WorMatri.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	WorMatri.Usage = D3D11_USAGE_DYNAMIC;

	D3D11_SUBRESOURCE_DATA DATA;

	ZeroMemory(&DATA, sizeof(DATA));
	DATA.pSysMem = &matrix;

	device->CreateBuffer(&WorMatri, &DATA, &MatrixBuffer);
	deviceContext->VSSetConstantBuffers(0, 1, &MatrixBuffer);

	LightSun();

	//Light buffer
	D3D11_BUFFER_DESC SunData;
	SunData.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	SunData.ByteWidth = sizeof(DirectionalLight);
	SunData.MiscFlags = 0;
	SunData.StructureByteStride = 0;
	SunData.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	SunData.Usage = D3D11_USAGE_DYNAMIC;

	D3D11_SUBRESOURCE_DATA LightData;
	LightData.pSysMem = &Sun;

	device->CreateBuffer(&SunData, &LightData, &LightBuffer);

	XMVECTOR sunPos, lightPointTo, up;
	sunPos = XMLoadFloat3(&Sun.SunPosition);
	up = XMLoadFloat3(&XMFLOAT3(0, 0, 1));
	lightPointTo = XMVector3Normalize(sunPos);

	XMStoreFloat4x4(&sunMatrix.View, XMMatrixTranspose(XMMatrixLookAtLH(sunPos, -lightPointTo, -up)));
	XMStoreFloat4x4(&sunMatrix.World, XMMatrixTranspose(XMMatrixScaling(1.0, 1.0, 1.0)));
	XMStoreFloat4x4(&sunMatrix.Proj, XMMatrixOrthographicLH(400, 400, 0.5, 800/*XMMatrixPerspectiveFovLH(XM_PI / 3, 1040.0f / 800.0f, 10.0f, 300.0f)*/));
	
	//matrixbuffer
	ZeroMemory(&DATA, sizeof(DATA));
	DATA.pSysMem = &sunMatrix;

	device->CreateBuffer(&WorMatri, &DATA, &SunBuffer);

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
	//gameTime.ShowFPS();

	DeferedRendering.setGBufferShaders(deviceContext);
	DeferedRendering.clearBuffer(deviceContext);

	cam.Update();
	//UPPDATING MATRIXBUFFER
	//Check for input
	cam.Input(gameTime.DeltaTime(), mainHwnd.getHWND());

	//--------------UPDATING MATRIXES-----------------------------
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
	temp->World = matrix.World;
	temp->View =  matrix.View;//change later depending on what object rendering? 
	temp->Proj = matrix.Proj;

	deviceContext->Unmap(MatrixBuffer, 0);
	//--------------UPDATING MATRIXES-----------------------------

	DeferedRendering.OMSetRender(device, deviceContext, directX.getDepthView(deviceContext));

	//draw obj

	hMap.render(deviceContext);
	obj.render(deviceContext);


	//Shadow
	deviceContext->VSSetConstantBuffers(0, 1, &SunBuffer);
	shadow.prepRun(deviceContext);
	hMap.render(deviceContext);
	obj.render(deviceContext);
	shadow.close(deviceContext);
	//deviceContext->VSSetConstantBuffers(0, 1, &MatrixBuffer);
	DeferedRendering.setShaderResources(deviceContext);


	//SSAO
	XMFLOAT4X4 oTemp;

	XMStoreFloat4x4(&oTemp, XMMatrixMultiplyTranspose(cam.GetViewMa(), cam.GetProjMa()));
	
	Ssao.renderPass(device, deviceContext, oTemp);

	//Finalising and drawing
	DeferedRendering.setBackBufferShaders(deviceContext);

	//Rendertarget
	DeferedRendering.OMSetBackBuff(deviceContext);

	deviceContext->PSSetConstantBuffers(0, 1, &LightBuffer);

	DeferedRendering.Render(device, deviceContext);

	//kolla shadowmappen
	swapChain->Present(0, 0);

	XMFLOAT3 posT = cam.getPos();

	std::cout <<"Curreny Position: X:"<< posT.x<<" Y: " <<posT.y<<"Z: "<< posT.z<< endl;

	DeferedRendering.CloseBuffers(deviceContext);
}

void GameSystem::LightSun()
{
	Sun.ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 0.5f);
	Sun.Diffuse = XMFLOAT4(0.5f, 0.5f, 0.5f, 0.5f);
	Sun.Specular = XMFLOAT4(0.2f, 0.2f, 0.2f, 0.2f);
	Sun.SunPosition = XMFLOAT3(150, 300, 150);
	Sun.mp = 0;
}

HWND GameSystem::getMainHwnd()
{
	return mainHwnd.getHWND();
}