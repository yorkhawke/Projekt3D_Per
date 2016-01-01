#include "QuadTree.h"

QuadTree::QuadTree()
{

}

QuadTree::~QuadTree()
{

}

void QuadTree::Initialzie(UINT* Ind, int NrIn, ID3D11Device* Device)
{
	indices = Ind;
	nrIndices = NrIn;
	UINT* temp1 = new UINT[NrIn / 4];
	UINT* temp2 = new UINT[NrIn / 4];
	UINT* temp3 = new UINT[NrIn / 4];
	UINT* temp4 = new UINT[NrIn / 4];
	int toNext = 0;
	Children = new QuadTree[4];

	if (NrFrustDetail < 5)
	{
		for (int i = 0;i < NrIn;i++)
		{
			if (i < NrIn / 2)
			{
				if (i < NrIn / 4)
					temp1[i] = Ind[i];
				else
					temp2[i] = Ind[i];
			}
			else
			{
				if (i < NrIn * 3 / 4)
					temp3[i] = Ind[i];
				else
					temp4[i] = Ind[i];
			}

		}


		Children[0].Initialzie(temp1, NrIn / 4, Device);
		Children[1].Initialzie(temp2, NrIn / 4, Device);
		Children[2].Initialzie(temp3, NrIn / 4, Device);
		Children[3].Initialzie(temp4, NrIn / 4, Device);
		NrFrustDetail++;
	}
	else (NrFrustDetail == 5)
	{
		
	}
	else
	{

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

}

void QuadTree::Render(ID3D11DeviceContext* DeviceContext)
{
	if (CheckFrustum())
	{
		DeviceContext->IASetIndexBuffer(IndexB, DXGI_FORMAT_R32_UINT, 0);
		DeviceContext->DrawIndexed(nrIndices, 0, 0);
	}
	else
	{

	}
}

bool QuadTree::CheckFrustum()
{
	
}
