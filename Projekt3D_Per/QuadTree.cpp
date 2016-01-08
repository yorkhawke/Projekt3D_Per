#include "QuadTree.h"

QuadTree::QuadTree()
{

}

QuadTree::~QuadTree()
{
	delete[] indices;
	delete[] vertexs;

	//IndexB->Release();
	//VertexB->Release();

	

}

void QuadTree::Initialzie(UINT* Ind, int NrIn, ID3D11Device* Device, UINT m, UINT n, Vertex* vertex,int layer,int ext,XMFLOAT3 Center)
{
	//indices = Ind;
	//nrIndices = NrIn;
	vertexs = vertex;
	nrVertexes = m*n;
	//UINT* temp1 = new UINT[NrIn / 4];
	//UINT* temp2 = new UINT[NrIn / 4];
	//UINT* temp3 = new UINT[NrIn / 4];
	//UINT* temp4 = new UINT[NrIn / 4];

	Vertex* Vtemp1 = new Vertex[m*n / 4];
	Vertex* Vtemp2 = new Vertex[m*n / 4];
	Vertex* Vtemp3 = new Vertex[m*n / 4];
	Vertex* Vtemp4 = new Vertex[m*n / 4];

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
	for (int i = 0; i < m / 2; ++i)
	{
		for (int j = 0; j < n / 2; ++j)
		{
			Vtemp1[added++] = vertex[i * m + j];
		}
	}

	// B
	added = 0;
	for (int i = 0; i < m / 2; ++i)
	{
		for (int j = n / 2; j < n; ++j)
		{
			Vtemp2[added++] = vertex[i * m + j];
		}
	}

	// C
	added = 0;
	for (int i = m / 2; i < m; ++i)
	{
		for (int j = 0; j < n / 2; ++j)
		{
			Vtemp3[added++] = vertex[i * m + j];
		}
	}

	// D
	added = 0;
	for (int i = m / 2; i < m; ++i)
	{
		for (int j = n / 2; j < n; ++j)
		{
			Vtemp4[added++] = vertex[i * m + j];
		}
	}

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

	if (layer!=0)
	{
		Children[0].Initialzie(indices, nrIndices, Device, m / 2, n / 2, Vtemp1, layer - 1, ext / 2, XMFLOAT3(Center.x - ext / 2, 0.0, Center.z + ext / 2));
		Children[1].Initialzie(indices, nrIndices, Device, m / 2, n / 2, Vtemp2, layer - 1, ext / 2, XMFLOAT3(Center.x + ext / 2, 0.0, Center.z + ext / 2));
		Children[2].Initialzie(indices, nrIndices, Device, m / 2, n / 2, Vtemp3, layer - 1, ext / 2, XMFLOAT3(Center.x - ext / 2, 0.0, Center.z - ext / 2));
		Children[3].Initialzie(indices, nrIndices, Device, m / 2, n / 2, Vtemp4, layer - 1, ext / 2, XMFLOAT3(Center.x + ext / 2, 0.0, Center.z - ext / 2));
	}
	else
	{
		leaf = true;
		delete[] indices;
		indices = Ind;
		nrIndices = NrIn;
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
}

void QuadTree::Render(ID3D11DeviceContext* DeviceContext, const XMMATRIX &projection, const XMMATRIX &view, const XMMATRIX &World)
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
				Children[i].Render(DeviceContext, projection,view,World);
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
				Children[i].Render(DeviceContext, projection, view, World);
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

