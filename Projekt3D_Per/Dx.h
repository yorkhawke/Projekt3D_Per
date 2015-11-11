#ifndef DX_H
#define DX_H

#include "Globals.h"

class Dx
{
public:
	Dx();
	~Dx();
	//initialization 
	void init(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	void endDX();
	//Buffer
	ID3D11DepthStencilState* getDepthState();
	ID3D11DepthStencilView* getDepthView(ID3D11DeviceContext* devC);
	//Shaders

private:

	//mabye have device etc in gamesystem for ease of acces? 
	IDXGISwapChain* swapChain;
	ID3D11Texture2D* depthStencilBuffer;
	ID3D11DepthStencilState* depthStencilState;
	ID3D11DepthStencilView* depthStencilView;
	ID3D11RasterizerState* rasterState;

	ID3D11ShaderResourceView* ShaderDepth;

};
#endif