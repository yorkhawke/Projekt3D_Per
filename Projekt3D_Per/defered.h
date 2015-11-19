#ifndef DEFERED_H
#define DEFERED_H
#include "Globals.h"

class Defered
{
public:
	Defered();
	~Defered();
	void StartUp(ID3D11Device* dev, ID3D11DeviceContext *devCon, IDXGISwapChain* swapChain);
	//set rendertarget
	void OMSetRender(ID3D11Device* dev, ID3D11DeviceContext *devCon, ID3D11DepthStencilView* dpStencilView);
	//Buffer
	void createBuffers(ID3D11DeviceContext* devCon, ID3D11Device* dev);
	void clearBuffer(ID3D11DeviceContext *devCon);
	//Shaders
	void createShaders(ID3D11Device* dev, ID3D11DeviceContext *devCon);
	void setGBufferShaders(ID3D11DeviceContext* devCon);
	void setBackBufferShaders(ID3D11DeviceContext* devCon);
	void setShaderResources(ID3D11DeviceContext *devCon);
	void OMSetBackBuff(ID3D11DeviceContext *devCon);
	void Render(ID3D11Device* dev, ID3D11DeviceContext *devCon);
	void CloseBuffers(ID3D11DeviceContext* devCon);
	void test(ID3D11DeviceContext* devCon);
	void nullRender(ID3D11DeviceContext* devCon);

private:
	ID3D11VertexShader* GVertexShader;
	ID3D11PixelShader* GPixelShader;
	ID3D11InputLayout* GVertexLayout;
	//Final rendering pass
	ID3D11VertexShader* DefVertexShader;
	ID3D11PixelShader* DefPixelShader;
	ID3D11InputLayout* DefVertexLayout;
	ID3D11Texture2D* backB;
	ID3D11RenderTargetView* BackBuff=0;

	ID3D11RenderTargetView* renderTargetViewA[BufferC];
	ID3D11ShaderResourceView* ShaderResource[BufferC];
	ID3D11Texture2D* renderTargetTex[BufferC];

	ID3D11Buffer* ScreenBuff = 0;

};
#endif
