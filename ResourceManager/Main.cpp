#include<list>
#include<vector>
#include<string>
#include"Utility.h"
#include"DXApp.h"
#include"FileIO.h"
#include"Node.h"
#include"Link.h"
#include"GraphManager.h"
#include"Camera.h"
#include"Slider.h"


//set up an application class, inheriting from the framework
class ResourceManager: public DXApp
{
public:
	ResourceManager(HINSTANCE hi);
	~ResourceManager();
	void init();

private:
	//private methods
	void updateScene(float delta);
	void draw();
	void handleResize();
	void initFX();
	void buildLayouts();
	void setUpGeometry();
	void loadFont();
	void createTexture();
	ID3D10Effect* buildFX(std::wstring filename);

	//instance handle
	HINSTANCE hInstance;

	//Matrices
	D3DXMATRIX viewMatrix;
	D3DXMATRIX worldMatrix;
	D3DXMATRIX projMatrix;
	D3DXMATRIX wvpMatrix;

	//Input Layout
	ID3D10InputLayout* PosColLayout;
	ID3D10InputLayout* PosTexLayout;

	//FX interfaces
	ID3D10Effect* LinkFX;
	ID3D10Effect* NodeFX;

	//FX variables to update
	ID3D10EffectShaderResourceVariable* nodeFXDiffuseMap;
	ID3D10EffectMatrixVariable* pWVPMatrixLinks;
	ID3D10EffectMatrixVariable* pWVPMatrixNodes;

	//Scene Camera
	Camera camera;

	//debug object
	FileIO io;

	//display manager
	GraphManager graphManager;

	//Font for dynamic text
	ID3DX10Font* idFont; 

	//two vectors used to store geometry setup in
	//the setUpGeometry() method
	std::vector<D3DXMATRIX> positionList;
	std::vector<Link> linkList;
	
	//variables used to determine the
	//node's position on the graph
	int radius;
	float theta;
	float interval;

	//flag to redraw the node's textures
	bool redrawTextures;
	
	//slider sprite to adjust navigation speed
	Slider slider;
};

ResourceManager::ResourceManager(HINSTANCE hi):
hInstance(hi), theta(0)
{
	io.clearFile("Debug.txt");
	io.readFile("resource.txt");
	redrawTextures = true;
}

ResourceManager::~ResourceManager()
{
	FREE(idFont);
	FREE(LinkFX);
	FREE(NodeFX);
	FREE(PosColLayout);
	FREE(PosTexLayout);
}

void ResourceManager::setUpGeometry()
{
	theta = 0;
	interval = (float)(2*PI / graphManager.nodeList.size());
	radius = graphManager.nodeList.size();
	//for every node in the graph, create and store a world matrix for it so that it lies
	//on the circumferance of the circular graph, also registering its position within the 
	//node data structure itself
	for(std::list<Node*>::iterator it = graphManager.nodeList.begin(); it != graphManager.nodeList.end(); it++)
	{
		D3DXMATRIX newMatrix;
		int xCoord = radius * cosf(theta);
		int yCoord = radius * sinf(theta);

		D3DXMatrixTranslation(&newMatrix, xCoord, yCoord, 0);
		positionList.push_back(newMatrix);
		D3DXVec4Transform(&(*it)->position,	&D3DXVECTOR4(0, 0, 0, 1), &newMatrix);

		theta += interval;
	}

	//for every node in the graph, create and store a link between that node and its resource nodes
	for(std::list<Node*>::iterator it = graphManager.nodeList.begin(); it != graphManager.nodeList.end(); it++)
	{
		if((*it)->dependList.size() > 0)
		{
			for(std::list<Node*>::iterator itr = (*it)->dependList.begin(); itr != (*it)->dependList.end(); itr++)
			{
				Link newLink(mDevice, (*it)->position, (*itr)->position);
				linkList.push_back(newLink);
			}
		}
	}
}

void ResourceManager::init()
{
	DXApp::initD3D();
	//initialize FX variables
	initFX();
	//build input layouts
	buildLayouts();
	//init DirectInput
	initDInput(hInstance, mainWindow);
	//init graph manager
	graphManager.init(mDevice, io.getLineList());
	//update the existing nodes after reading the NodeStatus.txt file (status information from the GUI)
	graphManager.updateNodes();
	//init geometry to be drawn
	setUpGeometry();
	//init the font
	loadFont();
	//init slider
	slider.initSprite(mDevice);
	//init camera
	camera.init(D3DXVECTOR3(0, 0, -3 * (int)graphManager.nodeList.size()), 
		D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 1, 0));

	//build the projection matrix and start the game clock
	this->handleResize();
	gameTimer.start();

#ifdef DEBUG
	io.writeFile("Debug.txt", "All components initialized successfully");
#endif
}

void ResourceManager::loadFont()
{
	//setup font to draw
	D3DX10_FONT_DESC fontDesc;
	fontDesc.Height = 120;
	fontDesc.Width = 0;
	fontDesc.Weight = 0;
	fontDesc.MipLevels = 1;
	fontDesc.Italic = false;
	fontDesc.CharSet = DEFAULT_CHARSET;
	fontDesc.OutputPrecision = OUT_DEFAULT_PRECIS;
	fontDesc.Quality = DEFAULT_QUALITY;
	fontDesc.PitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	wcscpy(fontDesc.FaceName, L"Times New Roman");

	D3DX10CreateFontIndirect(mDevice, &fontDesc, &idFont);
}

void ResourceManager::handleResize()
{
	DXApp::handleResize();
	//if window resized, scene needs to rebuild it's projection matrix
	D3DXMatrixPerspectiveFovLH(&projMatrix, 0.25f*PI, (float)CLIENT_WIDTH/CLIENT_HEIGHT, 1.0f, 1000.0f);

	//if window resized, the slider needs to rebuild it's projection matrix
	slider.rebuildProjection(CLIENT_WIDTH, CLIENT_HEIGHT);
}

void ResourceManager::initFX()
{
	LinkFX = buildFX(L"Link.fx");
	pWVPMatrixLinks = LinkFX->GetVariableByName("wvpMatrix")->AsMatrix();
	NodeFX = buildFX(L"Nodes.fx");
	pWVPMatrixNodes = NodeFX->GetVariableByName("wvpMatrix")->AsMatrix();
	nodeFXDiffuseMap = NodeFX->GetVariableByName("diffuseMap")->AsShaderResource();
}

ID3D10Effect* ResourceManager::buildFX(std::wstring filename)
{
	DWORD shaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;
#ifdef DEBUG
	shaderFlags |= D3D10_SHADER_DEBUG;
#endif
	//object to store shader compilation errors
	ID3D10Blob * compilationErrors = 0;
	// load the effect file
	ID3D10Effect* newFX;
	HRESULT hr = D3DX10CreateEffectFromFile(filename.c_str(), 0, 0, "fx_4_0", shaderFlags, 0, mDevice, 0, 0, 
		&newFX, &compilationErrors, 0);

	//check for compilation errors when loading the effect file
	if(compilationErrors)
		MessageBoxA(0, (char*)compilationErrors->GetBufferPointer(), 0, 0);

	return newFX;
}

void ResourceManager::buildLayouts()
{
	D3D10_INPUT_ELEMENT_DESC LinkVertexDesc[] = 
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0},
	};

	D3D10_PASS_DESC passDesc;
	LinkFX->GetTechniqueByName("T0")->GetPassByIndex(0)->GetDesc(&passDesc);
	mDevice->CreateInputLayout(LinkVertexDesc, 2, passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize, &PosColLayout);

	D3D10_INPUT_ELEMENT_DESC NodeVertexDesc[] = 
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD",  0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0},
	};

	NodeFX->GetTechniqueByName("T0")->GetPassByIndex(0)->GetDesc(&passDesc);
	mDevice->CreateInputLayout(NodeVertexDesc, 2, passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize, &PosTexLayout);
}

//overwrite the update and draw methods
void ResourceManager::updateScene(float delta)
{
	DXApp::updateScene(delta);
	//read input
	detectInput();

	//update slider
	slider.update(keystate, delta);

	//update the camera with the new input
	camera.Update(keystate, mouseState, delta, (int)slider.sliderPosX);

	//set local copy of view matrix to the camera view
	viewMatrix = camera.GetCameraView();
}

void ResourceManager::draw()
{
	DXApp::draw();

	//if there is a change in the graph data, redraw the textures of each node
	//using render to texture and the node's id
	if(redrawTextures)
	{
		for(std::list<Node*>::iterator it = graphManager.nodeList.begin(); it != graphManager.nodeList.end(); it++)
		{
			ID3D10RenderTargetView* renderTargets[1] = {(*it)->textureRTV};
			mDevice->OMSetRenderTargets(1, renderTargets, dsView);

			if((*it)->enabled)
				mDevice->ClearRenderTargetView((*it)->textureRTV, BLACK);
			else
				mDevice->ClearRenderTargetView((*it)->textureRTV, RED);

			std::wostringstream textToDraw;
			textToDraw << (*it)->nodeID.c_str();

			RECT randomRect = {200, CLIENT_HEIGHT / 2, 5, 5};
			idFont->DrawText(0, textToDraw.str().c_str(), -1, &randomRect, DT_NOCLIP, WHITE); 

			mDevice->GenerateMips((*it)->textureSRV);
		}

		redrawTextures = false;
	}

	//reset render target to back buffer
	mDevice->OMSetRenderTargets(1, &rtView, dsView);

	mDevice->IASetInputLayout(PosTexLayout);
	mDevice->OMSetDepthStencilState(0, 0);
	mDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	ID3D10EffectPass* NodePass = NodeFX->GetTechniqueByName("T0")->GetPassByIndex(0);
	ID3D10EffectPass* LinkPass = LinkFX->GetTechniqueByName("T0")->GetPassByIndex(0);

	//for every node in the graph, set a world-view-projection matrix for it based on its 
	//stored position on the graph (calculated in setUpGeometry()), and then render it in that position
	int i = 0;
	for(std::list<Node*>::iterator it = graphManager.nodeList.begin(); it != graphManager.nodeList.end(); it++)
	{
		nodeFXDiffuseMap->SetResource((*it)->textureSRV);//pass texture resource view
		wvpMatrix = positionList[i] * viewMatrix * projMatrix;
		pWVPMatrixNodes->SetMatrix(wvpMatrix);
		NodePass->Apply(0);
		(*it)->draw();
		i++;
	}

	//prepare for link draws
	mDevice->IASetInputLayout(PosColLayout);
	mDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP);
	D3DXMatrixIdentity(&worldMatrix);
	wvpMatrix = worldMatrix * viewMatrix * projMatrix;
	pWVPMatrixLinks->SetMatrix(wvpMatrix);

	//render every stored link that was created in setUpGeometry()
	for(std::vector<Link>::iterator it = linkList.begin(); it!=linkList.end(); it++)
	{
		LinkPass->Apply(0);
		it->draw();
	}

	//draw slider after 3D scene as been rendered
	slider.draw();

	mSwapChain->Present(0, 0);
}

//MAIN
int WINAPI WinMain(HINSTANCE hi, HINSTANCE hpi, PSTR pcl, int sc)
{
	ResourceManager rm(hi);
	if(!rm.initWindow(hi, sc))
		return 0;
	rm.init();
	return rm.run();
}