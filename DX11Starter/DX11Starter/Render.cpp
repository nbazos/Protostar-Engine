#include "Render.h"



Render::Render()
{
	//// Initialize fields
	//vertexShader = 0;
	//pixelShader = 0;
	//camera = 0;
	//material1 = 0;
	//material2 = 0;
	//brickSRV = 0;
	//grassSRV = 0;
	//sampler = 0;
}


Render::~Render()
{
	//// Delete our simple shader objects, which
	//// will clean up their own internal DirectX stuff
	//delete vertexShader;
	//delete pixelShader;
	//delete camera;

	//delete material1;
	//delete material2;
	//brickSRV->Release();
	//grassSRV->Release();
	//sampler->Release();

	//// Delete Meshes & GameEntities
	//for (auto& m : meshes) delete m;
	//for (auto& e : gameEntities) delete e;
}

void Render::Init()
{
	// Helper methods for loading shaders, creating some basic
	// geometry to draw and some simple camera matrices.
	//  - You'll be expanding and/or replacing these later
	/*LoadShaders();
	CreateMaterials();
	CreateBasicGeometry();*/

	// Create camera & initial projection matrix
	/*camera = new Camera(0.0f, 0.0f, -5.0f);
	camera->UpdateProjectionMatrix((float)width / height);*/

	// Define directional lights for the scene
	/*dirLight1 = { XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f), XMFLOAT4(0.75f, 0.75f, 0.75f, 1.0f), XMFLOAT3(1.0f, -1.0f, 0.0f) };
	dirLight2 = { XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f), XMFLOAT4(0.0f, 0.0f, 0.5f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f) };*/

	// Tell the input assembler stage of the pipeline what kind of
	// geometric primitives (points, lines or triangles) we want to draw.  
	// Essentially: "What kind of shape should the GPU draw with our data?"
	// context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Render::Update()
{
	// updateCamera()
	// camera->Update(deltaTime);

	// updateSceneElements()

	// renderScene()

	// swapBuffers()
}

void Render::Draw(float deltatime, float totalTime)
{
	//// Background color (Cornflower Blue in this case) for clearing
	//const float color[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	//// Clear the render target and depth buffer (erases what's on the screen)
	////  - Do this ONCE PER FRAME
	////  - At the beginning of Draw (before drawing *anything*)
	//context->ClearRenderTargetView(backBufferRTV, color);
	//context->ClearDepthStencilView(
	//	depthStencilView,
	//	D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
	//	1.0f,
	//	0);

	//pixelShader->SetData(
	//	"light1",
	//	&dirLight1,
	//	sizeof(DirectionalLight));
	//pixelShader->SetData(
	//	"light2",
	//	&dirLight2,
	//	sizeof(DirectionalLight));

	//for (int i = 0; i < gameEntities.size(); i++)
	//{
	//	gameEntities[i]->Draw(camera->GetViewMatrix(), camera->GetProjectionMatrix());
	//}

	//// Present the back buffer to the user
	////  - Puts the final frame we're drawing into the window so the user can see it
	////  - Do this exactly ONCE PER FRAME (always at the very end of the frame)
	//swapChain->Present(0, 0);
}

void Render::LoadShaders()
{
	/*vertexShader = new SimpleVertexShader(device, context);
	vertexShader->LoadShaderFile(L"VertexShader.cso");

	pixelShader = new SimplePixelShader(device, context);
	pixelShader->LoadShaderFile(L"PixelShader.cso");*/
}

void Render::CreateMaterials()
{
	//// Load textures
	//CreateWICTextureFromFile(
	//	device,					// The Direct3D device for resource creation
	//	context,				// Rendering context (this will auto-generate mip maps!!!)
	//	L"../../Assets/Textures/round_brick.jpg",	// Path to the file ("L" means wide characters)
	//	0,						// Texture ref?  No thanks!  (0 means we don't want an extra ref)
	//	&brickSRV);				// Actual SRV for use with shaders

	//CreateWICTextureFromFile(
	//	device,					// The Direct3D device for resource creation
	//	context,				// Rendering context (this will auto-generate mip maps!!!)
	//	L"../../Assets/Textures/grass.jpg",	// Path to the file ("L" means wide characters)
	//	0,						// Texture ref?  No thanks!  (0 means we don't want an extra ref)
	//	&grassSRV);

	//// Create sampler state
	//D3D11_SAMPLER_DESC samplerDesc = {};
	//samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	//samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	//samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	//samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	//samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	//samplerDesc.MaxAnisotropy = 16;
	//device->CreateSamplerState(&samplerDesc, &sampler);

	//material1 = new Material(vertexShader, pixelShader, brickSRV, sampler);
	//material2 = new Material(vertexShader, pixelShader, grassSRV, sampler);
}

void Render::CreateBasicGeometry()
{
	//Mesh* torusMesh = new Mesh("../../Assets/Models/torus.obj", device); // create triangle mesh
	//Mesh* cubeMesh = new Mesh("../../Assets/Models/cube.obj", device);
	//Mesh* coneMesh = new Mesh("../../Assets/Models/cone.obj", device);
	//Mesh* helixMesh = new Mesh("../../Assets//Models/helix.obj", device);
	//Mesh* sphereMesh = new Mesh("../../Assets/Models/sphere.obj", device);
	//meshes.push_back(torusMesh);
	//meshes.push_back(cubeMesh);
	//meshes.push_back(coneMesh);
	//meshes.push_back(helixMesh);
	//meshes.push_back(sphereMesh);

	//// Create GameEntities that utilize the meshes
	//GameEntity* torus = new GameEntity(torusMesh, material1, context);
	//GameEntity* cube = new GameEntity(cubeMesh, material1, context);
	//GameEntity* cone = new GameEntity(coneMesh, material1, context);
	//GameEntity* helix = new GameEntity(helixMesh, material2, context);
	//GameEntity* sphere = new GameEntity(sphereMesh, material2, context);
	//gameEntities.push_back(torus);
	//gameEntities.push_back(cube);
	//gameEntities.push_back(cone);
	//gameEntities.push_back(helix);
	//gameEntities.push_back(sphere);

	//// Set transformations
	//torus->SetPosition(-3, -1, 0);
	//torus->SetScale(0.5);
	//torus->SetWorldMatrix();

	//cube->SetScale(0.5);
	//cube->SetPosition(-1, 0, 0);
	//cube->SetRotation(0.0f, 45.0f, 0.0f);
	//cube->SetWorldMatrix();

	//cone->SetScale(0.5);
	//cone->SetPosition(2, -1, 0);
	//cone->SetWorldMatrix();

	//helix->SetScale(0.5);
	//helix->SetPosition(3, 1, 0);
	//helix->SetWorldMatrix();

	//sphere->SetScale(0.5);
	//sphere->SetPosition(0.5, 0, 0);
	//sphere->SetWorldMatrix();
}

void Render::AdjustCameraProjectionOnResize()
{
	// camera->UpdateProjectionMatrix((float)width / height);
}
