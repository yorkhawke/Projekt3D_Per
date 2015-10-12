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

void Defered::StartUp(ID3D11Device* dev, ID3D11DeviceContext *devCon)
{

}

//set rendertarget
void Defered::OMSetRender(ID3D11Device* dev, ID3D11DeviceContext *devCon, ID3D11DepthStencilView* dpStencilView)
{

}

//Buffer
void Defered::clearBuffer(ID3D11DeviceContext *devCon)
{

}

//Shaders
void Defered::createShaders(ID3D11Device* dev, ID3D11DeviceContext *devCon)
{

}

void Defered::setShaders(ID3D11DeviceContext* devCon)
{

}

void Defered::setShaderResources(ID3D11DeviceContext *devCon)
{

}
