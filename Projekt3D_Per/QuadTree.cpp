#include "QuadTree.h"

QuadTree::QuadTree()
{

}

QuadTree::~QuadTree()
{

}

void QuadTree::Initialzie(const XMMATRIX &projection,UINT* Ind, int NrIn, ID3D11Device* Device, UINT m, UINT n, Vertex* vertex,XMMATRIX &WorldM)
{
	indices = Ind;
	nrIndices = NrIn;
	vertexs = vertex;
	nrVertexes = m*n;
	XMFLOAT3* tempVertex = new XMFLOAT3[nrVertexes];//funkar nog itne :(

	if (NrFrustDetail == 5)
		leaf = true;

	if (NrFrustDetail < 5)
	{
	UINT* temp1 = new UINT[NrIn / 4];
	UINT* temp2 = new UINT[NrIn / 4];
	UINT* temp3 = new UINT[NrIn / 4];
	UINT* temp4 = new UINT[NrIn / 4];

	Vertex* Vtemp1 = new Vertex[m*n / 4];
	Vertex* Vtemp2 = new Vertex[m*n / 4];
	Vertex* Vtemp3 = new Vertex[m*n / 4];
	Vertex* Vtemp4 = new Vertex[m*n / 4];

	Children = new QuadTree[4];
		for (int i = 0; i < NrIn; i++)
		{

		}

		//VERTEXES
	

		for (int i = 0; i < m; i++)
		{
			for (int j = 0; j < n; j++)
			{
				if (i < m/2)
				{
					if (j < n/2)
						Vtemp1[j] = vertex[j*i];
	
					else
						Vtemp2[j] = vertex[j*i];


				}
				else
				{
					if (j < n / 2)
						Vtemp3[j] = vertex[j*i];

					else
						Vtemp4[j] = vertex[j*i];
				}
			}
		}

		NrFrustDetail++;
		Children[0].Initialzie(projection,temp1, NrIn / 4, Device, m / 2, n / 2, vertex,WorldM);
		Children[1].Initialzie(projection, temp2, NrIn / 4, Device, m / 2, n / 2, vertex, WorldM);
		Children[2].Initialzie(projection, temp3, NrIn / 4, Device, m / 2, n / 2, vertex, WorldM);
		Children[3].Initialzie(projection, temp4, NrIn / 4, Device, m / 2, n / 2, vertex, WorldM);

	}

	for (int i = 0;i < m*n;i++)
	{
		XMStoreFloat3(&tempVertex[i],XMVector3Transform(XMLoadFloat3(&vertex[i].Position),WorldM));
	}

		// indexBuffer
		D3D11_BUFFER_DESC IndexBufferDesc;
		IndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		IndexBufferDesc.ByteWidth = (6 * (m - 1)*(n - 1) * 4)+4;//+4 buffer //because there will be atleast one that gets a faulty value by about 4 bytes..
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

		//Bounding box
		BoundingBox test;
		box.CreateFromPoints(box,m*n,tempVertex,sizeof(XMFLOAT3));

}

void QuadTree::Render(ID3D11DeviceContext* DeviceContext, const XMMATRIX &projection)
{

	BoundingFrustum frust;//göra om frustumet till worldspace för boxarna...

	frust.CreateFromMatrix(frust, projection);
	int testValue = frust.Contains(box);
	switch (testValue)
	{
	case 0:
		break;
	case 1:
		if (!leaf)
		{
			for (int i = 0; i < 4; i++)
			{
				Children[i].Render(DeviceContext, projection);
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
	case 2:
		UINT32 vertexSize = sizeof(Vertex);
		UINT32 offset = 0;

		DeviceContext->IASetIndexBuffer(IndexB, DXGI_FORMAT_R32_UINT, 0);

		DeviceContext->IASetVertexBuffers(0, 1, &VertexB, &vertexSize, &offset);
		DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		DeviceContext->DrawIndexed(nrIndices, 0, 0);
		break;
	}
	//just to se if it renders...
	UINT32 vertexSize = sizeof(Vertex);
	UINT32 offset = 0;

	DeviceContext->IASetIndexBuffer(IndexB, DXGI_FORMAT_R32_UINT, 0);

	DeviceContext->IASetVertexBuffers(0, 1, &VertexB, &vertexSize, &offset);
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	DeviceContext->DrawIndexed(nrIndices,0, 0);
}

