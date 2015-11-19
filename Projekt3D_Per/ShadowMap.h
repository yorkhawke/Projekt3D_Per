#ifndef SHADOW_MAP
#define SHADWO_MAP
#include "Globals.h"

class ShadowMap
{
public:
	ShadowMap();
	~ShadowMap();

	void StartUp(ID3D11Device* device, ID3D11DeviceContext* devCon, Matrix light);
	void prepRun(ID3D11DeviceContext* devCon );
	void close(ID3D11DeviceContext* devCon);

private:
	ID3D11VertexShader* shadowMapVertexShader;
	ID3D11ShaderResourceView* ShaderDepth;
	ID3D11Texture2D* depthStencilBuffer;
	ID3D11DepthStencilState* depthStencilState;
	ID3D11DepthStencilView* depthStencilView;
};


#endif
