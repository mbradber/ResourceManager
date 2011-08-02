#include "Node.h"

Node::Node(ID3D10Device* device, std::string id, float scale):
pDevice(device), cubeScale(scale), nodeID(id)
{
	dependList.clear();
	buildCube();
	loadTexture();
}

void Node::buildCube()
{
	cube.init(pDevice, cubeScale);
}

void Node::draw()
{
	cube.draw();
}

void Node::loadTexture()
{
	ID3D10Texture2D* mTex = 0;
	D3D10_TEXTURE2D_DESC texDesc;

	texDesc.Width = 800;
	texDesc.Height = 600;
	texDesc.MipLevels = 0;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R11G11B10_FLOAT;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D10_USAGE_DEFAULT;
	texDesc.BindFlags = D3D10_BIND_RENDER_TARGET | D3D10_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = D3D10_RESOURCE_MISC_GENERATE_MIPS;

	pDevice->CreateTexture2D(&texDesc, 0, &mTex);

	pDevice->CreateRenderTargetView(mTex, 0, &textureRTV);

	pDevice->CreateShaderResourceView(mTex, 0, &textureSRV);

	if(mTex)
	{
		mTex->Release();
		mTex = NULL;
	}
}

Node::~Node(void)
{
	FREE(textureSRV);
	FREE(textureRTV);
}
