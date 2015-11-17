#ifndef SHADOW_MAP
#define SHADWO_MAP
#include "Globals.h"

class ShadowMap
{
public:
	ShadowMap();
	~ShadowMap();

	void StartUp(ID3D11Device* device, XMFLOAT3 Sun, XMFLOAT4X4 lightView);
	void Render(ID3D11DeviceContext* devCon );

private:
	Matrix shadowMatrix;

	ID3D11VertexShader* shadowMapVertexShader;
	ID3D11Buffer* lightBuffer;
	ID3D11Buffer* MatrixBuffer;
	ID3D11InputLayout* Layout;
	ID3D11ShaderResourceView* ShaderDepth;
	ID3D11Texture2D* depthStencilBuffer;
	ID3D11DepthStencilState* depthStencilState;
	ID3D11DepthStencilView* depthStencilView;
};


#endif
