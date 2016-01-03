#include "QuadTree.h"

QuadTree::QuadTree()
{

}

QuadTree::~QuadTree()
{

}

void QuadTree::Initialzie(UINT* Ind, int NrIn, ID3D11Device* Device, UINT m, UINT n, Vertex* vertex,XMMATRIX proj)
{
	indices = Ind;
	nrIndices = NrIn;
	vertexs = vertex;
	UINT* temp1 = new UINT[NrIn / 4];
	UINT* temp2 = new UINT[NrIn / 4];
	UINT* temp3 = new UINT[NrIn / 4];
	UINT* temp4 = new UINT[NrIn / 4];

	Vertex* Vtemp1 = new Vertex[m*n / 4];
	Vertex* Vtemp2 = new Vertex[m*n / 4];
	Vertex* Vtemp3 = new Vertex[m*n / 4];
	Vertex* Vtemp4 = new Vertex[m*n / 4];
	XMFLOAT3* tempVertex = new XMFLOAT3[m*n];
	Children = new QuadTree[4];
	if (NrFrustDetail < 5)
	{

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
				tempVertex[i*j] = vertex[i*j].Position;
			}
		}


		Children[0].Initialzie(temp1, NrIn / 4, Device, m / 2, n / 2, vertex,proj);
		Children[1].Initialzie(temp2, NrIn / 4, Device, m / 2, n / 2, vertex, proj);
		Children[2].Initialzie(temp3, NrIn / 4, Device, m / 2, n / 2, vertex, proj);
		Children[3].Initialzie(temp4, NrIn / 4, Device, m / 2, n / 2, vertex, proj);
		NrFrustDetail++;
	}

		// indexBuffer
		D3D11_BUFFER_DESC IndexBufferDesc;
		IndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		IndexBufferDesc.ByteWidth = NrIn;
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
		frust.CreateFromMatrix(frust, proj);

}

void QuadTree::Render(ID3D11DeviceContext* DeviceContext)
{
	if (CheckFrustum())
	{
		for (int i = 0; i < 4; i++)
		{
			Children[i].Render(DeviceContext);//need to render only the children.... then end this shit
		}

	}
	else
	{
		return;
	}

	UINT32 vertexSize = sizeof(Vertex);
	UINT32 offset = 0;

	DeviceContext->IASetIndexBuffer(IndexB, DXGI_FORMAT_R32_UINT, 0);

	DeviceContext->IASetVertexBuffers(0, 1, &VertexB, &vertexSize, &offset);
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	DeviceContext->DrawIndexed(nrIndices, 0, 0);
}

bool QuadTree::CheckFrustum()
{
	return true;
}
