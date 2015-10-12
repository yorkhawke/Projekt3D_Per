#ifndef DEFERED_H
#define DEFERED_H
#include "Globals.h"

class Defered
{
public:
	Defered();
	~Defered();
	void StartUp(ID3D11Device* dev, ID3D11DeviceContext *devCon);
	//set rendertarget
	void OMSetRender(ID3D11Device* dev, ID3D11DeviceContext *devCon, ID3D11DepthStencilView* dpStencilView);
	//Buffer
	void clearBuffer(ID3D11DeviceContext *devCon);
	//Shaders
	void createShaders(ID3D11Device* dev, ID3D11DeviceContext *devCon);
	void setShaders(ID3D11DeviceContext* devCon);
	void setShaderResources(ID3D11DeviceContext *devCon);

private:
	ID3D11VertexShader* GVertexShader;
	ID3D11PixelShader* GPixelShader;
	ID3D11InputLayout* GVertexLayout;

	ID3D11RenderTargetView* renderTargetViewA[BufferC];
	ID3D11ShaderResourceView* ShaderResource[BufferC];
	ID3D11Texture2D* renderTargetTex[BufferC];
};
#endif
