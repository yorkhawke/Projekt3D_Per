#ifndef QUADTREE_H
#define QUADTREE_H
#include "Globals.h"
#include "Cam.h"

class QuadTree
{
private:
	UINT* indices; //Indices array
	int nrIndices;// number of indices
	bool render = true;
	QuadTree* Children;//mah Chiledreren
	ID3D11Buffer* IndexB;
	ID3D11Buffer* VertexB;
	Vertex* vertexs;
public:
	QuadTree();
	~QuadTree();
	void Initialzie(const XMMATRIX &projection,UINT* Ind, int NrIn, ID3D11Device* Device, UINT m, UINT n, Vertex* vertex);
	void Render(ID3D11DeviceContext* DeviceContext, const XMMATRIX &projection);
	int CheckFrustum();

protected:
	BoundingBox box;
	BoundingFrustum frust;
};

#endif
