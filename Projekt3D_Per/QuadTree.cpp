#include "QuadTree.h"

QuadTree::QuadTree()
{
	IndexB = 0;
	VertexB = 0;
	indices = 0;
	vertexs = 0;
	Children = 0;
}

QuadTree::~QuadTree()
{

	//delete vertexs;
	delete[] indices;
	delete[] vertexs;
	if (leaf)
	{
		IndexB->Release();
		VertexB->Release();
		
	}

	delete[] Children;
}

void QuadTree::Initialzie(UINT* Ind, int NrIn, ID3D11Device* Device, UINT m, UINT n, Vertex* vertex,int layer,int ext,XMFLOAT3 Center)
{
	//indices = Ind;
	//nrIndices = NrIn;
	vertexs = new Vertex[m*n];

	for (int i = 0; i < m*n; i++)
	{
		vertexs[i] = vertex[i];
	}
	nrVertexes = m*n;


	Vertex* Vtemp1 = new Vertex[m*n / 4+m+n];
	Vertex* Vtemp2 = new Vertex[m*n / 4+m+n];
	Vertex* Vtemp3 = new Vertex[m*n / 4+m+n];
	Vertex* Vtemp4 = new Vertex[m*n / 4+m+n];

	Children = new QuadTree[4];


		

	//VERTEXES

	//  ___________________   ^
	// |         |         |  |
	// |    A    |    B    |  |
	// |         |         |  |
	// |---------|---------|  m
	// |         |         |  |
	// |    C    |    D    |  |
	// |_________|_________|  v
	// <---------n--------->

	// A
	int added = 0;
	for (int i = 0; i <= m / 2; ++i)
	{
		for (int j = 0; j <= n / 2; ++j)
		{
			Vtemp1[added++] = vertex[i * m + j];
		}
	}

	// B
	added = 0;
	for (int i = 0; i <= m / 2; ++i)
	{
		for (int j = n / 2-1; j < n; ++j)
		{
			Vtemp2[added++] = vertex[i * m + j];
		}
	}

	// C
	added = 0;
	for (int i = m / 2-1; i < m; ++i)
	{
		for (int j = 0; j <= n / 2; ++j)
		{
			Vtemp3[added++] = vertex[i * m + j];
		}
	}

	// D
	added = 0;
	for (int i = m / 2-1; i < m; ++i)
	{
		for (int j = n / 2-1; j < n; ++j)
		{
			Vtemp4[added++] = vertex[i * m + j];
		}
	}


	if (layer!=0)
	{
		int faceCount = (m / 2 - 1)*(n / 2 - 1) * 2;
		nrIndices = faceCount * 3;
		int k = 0;
		indices = new UINT[nrIndices];

		for (int i = 0; i < m / 2 - 1; i++)
		{
			for (int j = 0; j < n / 2 - 1; j++)
			{
				indices[k] = i*(n / 2) + j;
				indices[k + 1] = i*(n / 2) + j + 1;
				indices[k + 2] = (i + 1)*(n / 2) + j;
				indices[k + 3] = (i + 1)*(n / 2) + j;
				indices[k + 4] = i*(n / 2) + j + 1;
				indices[k + 5] = (i + 1)*(n / 2) + j + 1;
				k += 6;
			}
		}

		Children[0].Initialzie(indices, nrIndices, Device, m / 2, n / 2, Vtemp1, layer - 1, ext / 2, XMFLOAT3(Center.x - ext / 2, 0.0, Center.z + ext / 2));
		Children[1].Initialzie(indices, nrIndices, Device, m / 2, n / 2, Vtemp2, layer - 1, ext / 2, XMFLOAT3(Center.x + ext / 2, 0.0, Center.z + ext / 2));
		Children[2].Initialzie(indices, nrIndices, Device, m / 2, n / 2, Vtemp3, layer - 1, ext / 2, XMFLOAT3(Center.x - ext / 2, 0.0, Center.z - ext / 2));
		Children[3].Initialzie(indices, nrIndices, Device, m / 2, n / 2, Vtemp4, layer - 1, ext / 2, XMFLOAT3(Center.x + ext / 2, 0.0, Center.z - ext / 2));
	}
	else
	{
	leaf = true;
	nrIndices = NrIn;
	indices = new UINT[nrIndices];
	for (int i = 0; i < NrIn; i++)
	{
		indices[i] = Ind[i];
	}

	//indices = Ind;
	// indexBuffer
	D3D11_BUFFER_DESC IndexBufferDesc;
	IndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	IndexBufferDesc.ByteWidth = nrIndices*sizeof(UINT);
	IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IndexBufferDesc.CPUAccessFlags = 0;
	IndexBufferDesc.MiscFlags = 0;
	IndexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA Indexdata;
	Indexdata.pSysMem = indices;
	Device->CreateBuffer(&IndexBufferDesc, &Indexdata, &IndexB);

	D3D11_BUFFER_DESC vbdesc;

	ZeroMemory(&vbdesc, sizeof(vbdesc));

	vbdesc.Usage = D3D11_USAGE_DEFAULT;
	vbdesc.ByteWidth = sizeof(Vertex)*m*n;
	vbdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbdesc.CPUAccessFlags = 0;
	vbdesc.MiscFlags = 0;
	vbdesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA Data;
	ZeroMemory(&Data, sizeof(Data));
	Data.pSysMem = vertexs;

	Device->CreateBuffer(&vbdesc, &Data, &VertexB);
	}

	BoundingBox testBox(Center, XMFLOAT3(ext+5, 10, ext+5));
	BoundingSphere testrund(Center, ext+5);

	box = testBox;
	sphere = testrund;

	delete[] Vtemp1;
	delete[] Vtemp2;
	delete[] Vtemp3;
	delete[] Vtemp4;
}

void QuadTree::Render(ID3D11DeviceContext* DeviceContext, const XMMATRIX &projection, const XMMATRIX &view)
{
	//Frust på Projection sen multiplicera med inverse proj inverse view iverse world.


	BoundingFrustum frust;//göra om frustumet till worldspace för boxarna...

	frust.CreateFromMatrix(frust, projection);
	frust.Transform(frust, XMMatrixInverse(nullptr, view));//FUNKAR TESTAT!

	//if (frust.Intersects(box))
	//{
	//		if (!leaf)
	//		{
	//			for (int i = 0; i < 4; i++)
	//			{
	//				Children[i].Render(DeviceContext, projection,view,World);
	//			}
	//		}
	//		else
	//		{
	//			UINT32 vertexSize = sizeof(Vertex);
	//			UINT32 offset = 0;

	//			DeviceContext->IASetIndexBuffer(IndexB, DXGI_FORMAT_R32_UINT, 0);

	//			DeviceContext->IASetVertexBuffers(0, 1, &VertexB, &vertexSize, &offset);
	//			DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//			DeviceContext->DrawIndexed(nrIndices, 0, 0);
	//		}

	int testValue = frust.Contains(sphere);
	switch (testValue)
	{
	case 0:
		break;
	case 1:
		if (!leaf)
		{
			for (int i = 0; i < 4; i++)
			{
				Children[i].Render(DeviceContext, projection,view);
			}
		}
		else
		{
			UINT32 vertexSize = sizeof(Vertex);
			UINT32 offset = 0;

			DeviceContext->IASetIndexBuffer(IndexB, DXGI_FORMAT_R32_UINT, 0);

			DeviceContext->IASetVertexBuffers(0, 1, &VertexB, &vertexSize, &offset);
			DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			DeviceContext->DrawIndexed(nrIndices, 0, 0);
		}
		break;
	default:
		if (!leaf)
		{
			for (int i = 0; i < 4; i++)
			{
				Children[i].Render(DeviceContext, projection, view);
			}
		}
		else
		{
			UINT32 vertexSize = sizeof(Vertex);
			UINT32 offset = 0;

			DeviceContext->IASetIndexBuffer(IndexB, DXGI_FORMAT_R32_UINT, 0);

			DeviceContext->IASetVertexBuffers(0, 1, &VertexB, &vertexSize, &offset);
			DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			DeviceContext->DrawIndexed(nrIndices, 0, 0);
		}
		break;
	}
}

