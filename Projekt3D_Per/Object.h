#ifndef OBJECT_H
#define OBJECT_H

#include "Globals.h"

class Entity
{

public:

	Entity();
	~Entity();

	void createbuff(ID3D11Device* Device);

	void render(ID3D11DeviceContext* devCont);

	void createTexture(ID3D11Device* device, ID3D11DeviceContext* devCont, const wchar_t* filename);

protected:
	UINT* indices;
	Vertex* Vertices;
	ID3D11ShaderResourceView* Tex;
	ID3D11Buffer* VertexB;
	ID3D11Buffer* IndexB;
	int nrVertrices;
	int nrIndexes;
};

#endif