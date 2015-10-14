#include "Object.h"

Entity::Entity()
{
	VertexB = 0;
}

Entity::~Entity()
{
	Tex->Release();
	delete Vertices;
	delete indices;
}

void Entity::createbuff(ID3D11Device* Device)
{
	HRESULT hr;

	D3D11_BUFFER_DESC vbdesc;

	ZeroMemory(&vbdesc, sizeof(vbdesc));

	vbdesc.Usage = D3D11_USAGE_DEFAULT;
	vbdesc.ByteWidth = sizeof(Vertex)*nrVertrices;
	vbdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbdesc.CPUAccessFlags = 0;
	vbdesc.MiscFlags = 0;
	vbdesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA Data;
	ZeroMemory(&Data, sizeof(Data));
	Data.pSysMem = Vertices;

	hr = Device->CreateBuffer(&vbdesc, &Data, &VertexB);

}

void Entity::render(ID3D11DeviceContext* devCont)
{
	UINT32 vertexSize = sizeof(Vertex);
	UINT32 offset = 0;

	devCont->IASetIndexBuffer(IndexB, DXGI_FORMAT_R32_UINT, 0);

	devCont->IASetVertexBuffers(0, 1, &VertexB, &vertexSize, &offset);
	devCont->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	devCont->PSSetShaderResources(0, 1, &Tex);

	devCont->DrawIndexed(nrIndexes, 0, 0);
}

void Entity::createTexture(ID3D11Device* device, ID3D11DeviceContext* devCont, const wchar_t* filename)
{
	HRESULT hr;
	CoInitialize(NULL);
	hr = CreateWICTextureFromFile(device, devCont, filename , nullptr, &Tex);
}

