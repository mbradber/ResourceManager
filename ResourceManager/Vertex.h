#pragma once
#include"d3dx10.h"

struct PosColVertex
{
      D3DXVECTOR3 pos;
      D3DXCOLOR col;

	  PosColVertex(){}
	  PosColVertex(D3DXVECTOR3 p, D3DXCOLOR c){pos = p; col = c;}
};

struct PosTexVertex
{
	D3DXVECTOR3 pos;
	D3DXVECTOR2 texC;

	PosTexVertex(){}
	PosTexVertex(D3DXVECTOR3 p, D3DXVECTOR2 tex){pos = p; texC = tex;}
};

