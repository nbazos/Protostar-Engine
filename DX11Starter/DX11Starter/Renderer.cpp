#include "Renderer.h"

Renderer::Renderer(ID3D11DeviceContext*	ctx, ID3D11Device* d, unsigned int w, unsigned int h, ID3D11RenderTargetView* bBRTV, ID3D11DepthStencilView* dSV, IDXGISwapChain* sC, Camera * cam, SceneManager* sceneMg)
{
	context = ctx;
	device = d;
	width = w;
	height = h;
	backBufferRTV = bBRTV;
	depthStencilView = dSV;
	swapChain = sC;
	camera = cam;
	sceneManager = sceneMg;
	gameEntities = sceneManager->GetSceneEntities();

	vertexShader = 0;
	pixelShader = 0;
	skyVS = 0;
	skyPS = 0;
	particleVS = 0;
	particlePS = 0;
	material1 = 0;
	material2 = 0;
	brickSRV = 0;
	grassSRV = 0;
	skySRV = 0;
	particleSRV = 0;
	sampler = 0;
}

void Renderer::Init()
{
	LoadShaders();
	CreateMaterials();
	CreateBasicGeometry();

	// Define directional lights for the scene
	dirLight1 = { XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f), XMFLOAT4(0.75f, 0.75f, 0.75f, 1.0f), XMFLOAT3(0.0f, -1.0f, 1.0f) };
}

void Renderer::LoadShaders()
{
	vertexShader = new SimpleVertexShader(device, context);
	vertexShader->LoadShaderFile(L"VertexShader.cso");

	pixelShader = new SimplePixelShader(device, context);
	pixelShader->LoadShaderFile(L"PixelShader.cso");

	skyVS = new SimpleVertexShader(device, context);
	skyVS->LoadShaderFile(L"SkyVS.cso");

	skyPS = new SimplePixelShader(device, context);
	skyPS->LoadShaderFile(L"SkyPS.cso");

	particleVS = new SimpleVertexShader(device, context);
	particleVS->LoadShaderFile(L"ParticleVS.cso");

	particlePS = new SimplePixelShader(device, context);
	particlePS->LoadShaderFile(L"ParticlePS.cso");
}

void Renderer::CreateMaterials()
{
	// Load textures
	CreateWICTextureFromFile(
		device,					// The Direct3D device for resource creation
		context,				// Rendering context (this will auto-generate mip maps!!!)
		L"../../Assets/Textures/round_brick.jpg",	// Path to the file ("L" means wide characters)
		0,						// Texture ref?  No thanks!  (0 means we don't want an extra ref)
		&brickSRV);				// Actual SRV for use with shaders

	CreateWICTextureFromFile(
		device,					// The Direct3D device for resource creation
		context,				// Rendering context (this will auto-generate mip maps!!!)
		L"../../Assets/Textures/grass.jpg",	// Path to the file ("L" means wide characters)
		0,						// Texture ref?  No thanks!  (0 means we don't want an extra ref)
		&grassSRV);

	// Create sampler state
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.MaxAnisotropy = 16;
	device->CreateSamplerState(&samplerDesc, &sampler);

	material1 = new Material(vertexShader, pixelShader, brickSRV, sampler);
	material2 = new Material(vertexShader, pixelShader, grassSRV, sampler);

	// ---- Skybox ----
	// From: https://opengameart.org/content/cloudy-skyboxes
	CreateDDSTextureFromFile(device, L"../../Assets/Textures/SunnyCubeMap.dds", 0, &skySRV);

	// Create the states for the sky
	D3D11_RASTERIZER_DESC rdSky = {};
	rdSky.CullMode = D3D11_CULL_FRONT;
	rdSky.FillMode = D3D11_FILL_SOLID;
	rdSky.DepthClipEnable = true;
	device->CreateRasterizerState(&rdSky, &skyRasterState);

	D3D11_DEPTH_STENCIL_DESC dd = {};
	dd.DepthEnable = true;
	dd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dd.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	device->CreateDepthStencilState(&dd, &skyDepthState);

	// ---- Particles ----	
	CreateWICTextureFromFile(device, context, L"../../Assets/Textures/smokeParticle.png", 0, &particleSRV);

	// A depth state for the particles
	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO; // Turns off depth writing
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	device->CreateDepthStencilState(&dsDesc, &particleDepthState);

	// Blend for particles (additive)
	D3D11_BLEND_DESC blend = {};
	blend.AlphaToCoverageEnable = false;
	blend.IndependentBlendEnable = false;
	blend.RenderTarget[0].BlendEnable = true;
	blend.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blend.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA; // Still respect pixel shader output alpha
	blend.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	blend.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blend.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blend.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	blend.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	device->CreateBlendState(&blend, &particleBlendState);

	// Debug rasterizer state for particles
	D3D11_RASTERIZER_DESC rdPart = {};
	rdPart.CullMode = D3D11_CULL_BACK;
	rdPart.DepthClipEnable = true;
	rdPart.FillMode = D3D11_FILL_WIREFRAME;
	device->CreateRasterizerState(&rdPart, &particleDebugRasterState);

	// Set up particle emitters
	emitter1 = new ParticleEmitter(
		300,							// Max particles
		150,								// Particles per second
		2,								// Particle lifetime
		0.1f,							// Start size
		0.0f,							// End size
		XMFLOAT4(1, 1, 1, 0.2f),// Start color
		XMFLOAT4(1, 1, 1, 0.0f),// End color
		XMFLOAT3(0, 1, 0),				// Start velocity
		XMFLOAT3(0.2f, 0.5f, 0.0f),	// Velocity randomness range
		XMFLOAT3(0, 35, 20),			// Emitter position
		XMFLOAT3(0.0f, 0.0f, 0.2f),				// Position randomness range
		XMFLOAT4(0, 0, 0, 0),			// Random rotation ranges (startMin, startMax, endMin, endMax)
		XMFLOAT3(0, 1, 0),				// Constant acceleration
		device,
		particleVS,
		particlePS,
		particleSRV);
}

void Renderer::CreateBasicGeometry()
{
	Mesh* cubeMesh = new Mesh("../../Assets/Models/cube.obj", device);
	Mesh* playerMesh = new Mesh("../../Assets/Models/sphere.obj", device);
	meshes.push_back(cubeMesh);
	meshes.push_back(playerMesh);

	// Add player to the scene
	sceneManager->AddEntityToScene(GameEntity("Player", playerMesh, material2, context, XMFLOAT3(1, 3, 0), XMFLOAT3(0.5f, 0.5f, 0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f), 1.0f));

	CreateBasicLevel();
}

void Renderer::CreateBasicLevel()
{
	sceneManager->AddEntityToScene(GameEntity("Floor", meshes[0], material1, context, XMFLOAT3(15, -2.0f, 0), XMFLOAT3(10.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f));
	sceneManager->AddEntityToScene(GameEntity("Floor", meshes[0], material1, context, XMFLOAT3(35, 8.0f, 0), XMFLOAT3(10.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f));
	sceneManager->AddEntityToScene(GameEntity("Floor", meshes[0], material1, context, XMFLOAT3(52, -2.0f, 0), XMFLOAT3(10.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f));
	sceneManager->AddEntityToScene(GameEntity("Platform", meshes[0], material1, context, XMFLOAT3(20, 2.0f, 0), XMFLOAT3(3.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f));
	sceneManager->AddEntityToScene(GameEntity("Platform", meshes[0], material1, context, XMFLOAT3(27, 5.0f, 0), XMFLOAT3(3.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f));
	sceneManager->AddEntityToScene(GameEntity("Bouncy Platform", meshes[0], material1, context, XMFLOAT3(42, -2.0f, 0), XMFLOAT3(3.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f));
	sceneManager->AddEntityToScene(GameEntity("Crate", meshes[0], material1, context, XMFLOAT3(35, 9.0f, 0), XMFLOAT3(0.5f, 0.5f, 0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f), 10.0f));
	sceneManager->AddEntityToScene(GameEntity("Crate", meshes[0], material1, context, XMFLOAT3(35, 10.0f, 0), XMFLOAT3(0.5f, 0.5f, 0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f), 10.0f));
	sceneManager->AddEntityToScene(GameEntity("Crate", meshes[0], material1, context, XMFLOAT3(35, 11.0f, 0), XMFLOAT3(0.5f, 0.5f, 0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f), 10.0f));
	sceneManager->AddEntityToScene(GameEntity("Crate", meshes[0], material1, context, XMFLOAT3(35, 12.0f, 0), XMFLOAT3(0.5f, 0.5f, 0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f), 10.0f));
	sceneManager->AddEntityToScene(GameEntity("Crate", meshes[0], material1, context, XMFLOAT3(35, 13.0f, 0), XMFLOAT3(0.5f, 0.5f, 0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f), 10.0f));
	sceneManager->AddEntityToScene(GameEntity("Crate", meshes[0], material1, context, XMFLOAT3(35, 14.0f, 0), XMFLOAT3(0.5f, 0.5f, 0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f), 10.0f));
	sceneManager->AddEntityToScene(GameEntity("Crate", meshes[0], material1, context, XMFLOAT3(35, 15.0f, 0), XMFLOAT3(0.5f, 0.5f, 0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f), 10.0f));
	sceneManager->AddEntityToScene(GameEntity("Floor", meshes[0], material1, context, XMFLOAT3(0, -2.0f, 0), XMFLOAT3(10.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f));
}

void Renderer::Draw(float deltaTime, float totalTime)
{
	// Background color (Cornflower Blue in this case) for clearing
	const float color[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	// Clear the render target and depth buffer (erases what's on the screen)
	//  - Do this ONCE PER FRAME
	//  - At the beginning of Draw (before drawing *anything*)
	context->ClearRenderTargetView(backBufferRTV, color);
	context->ClearDepthStencilView(
		depthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);

	pixelShader->SetData(
		"light1",
		&dirLight1,
		sizeof(DirectionalLight));

	for (int i = 0; i < (*gameEntities).size(); i++)
	{
		(*gameEntities)[i].Draw(camera->GetViewMatrix(), camera->GetProjectionMatrix());
	}

	DrawSky();

	float blend[4] = { 1,1,1,1 };
	// Particle states
	context->OMSetBlendState(particleBlendState, blend, 0xffffffff);	// Additive blending
	context->OMSetDepthStencilState(particleDepthState, 0);				// No depth WRITING

	// No wireframe debug
	particlePS->SetInt("debugWireframe", 0);
	particlePS->CopyAllBufferData();

	// Draw the emitters
	emitter1->Draw(context, camera);

	if (GetAsyncKeyState('C'))
	{
		context->RSSetState(particleDebugRasterState);
		particlePS->SetInt("debugWireframe", 1);
		particlePS->CopyAllBufferData();

		emitter1->Draw(context, camera);
	}

	// Reset to default states for next frame
	context->OMSetBlendState(0, blend, 0xffffffff);
	context->OMSetDepthStencilState(0, 0);
	context->RSSetState(0);

	// Present the back buffer to the user
	//  - Puts the final frame we're drawing into the window so the user can see it
	//  - Do this exactly ONCE PER FRAME (always at the very end of the frame)
	swapChain->Present(0, 0);
}

void Renderer::DrawSky()
{
	ID3D11Buffer* vb = meshes[0]->GetVertexBuffer();
	ID3D11Buffer* ib = meshes[0]->GetIndexBuffer();

	// Set buffers in the input assembler
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &vb, &stride, &offset);
	context->IASetIndexBuffer(ib, DXGI_FORMAT_R32_UINT, 0);

	// Set up shaders
	skyVS->SetMatrix4x4("view", camera->GetViewMatrix());
	skyVS->SetMatrix4x4("projection", camera->GetProjectionMatrix());
	skyVS->CopyAllBufferData();
	skyVS->SetShader();
	skyPS->SetShaderResourceView("Sky", skySRV);
	skyPS->SetSamplerState("BasicSampler", sampler);
	skyPS->SetShader();

	// Set up sky-specific render states
	context->RSSetState(skyRasterState);
	context->OMSetDepthStencilState(skyDepthState, 0);

	// Draw
	context->DrawIndexed(meshes[0]->GetIndexCount(), 0, 0);

	// Reset your states
	context->RSSetState(0);
	context->OMSetDepthStencilState(0, 0);
}

void Renderer::ExitRenderer()
{
	delete vertexShader;
	delete pixelShader;
	delete skyVS;
	delete skyPS;
	delete particleVS;
	delete particlePS;
	delete material1;
	delete material2;
	delete emitter1;

	if (brickSRV != nullptr) { brickSRV->Release(); }
	if (grassSRV != nullptr) { grassSRV->Release(); }
	if (skySRV != nullptr) { skySRV->Release(); }
	if (particleSRV != nullptr) { particleSRV->Release(); }
	if (sampler != nullptr) { sampler->Release(); }

	for (auto& m : meshes) delete m;

	skyDepthState->Release();
	skyRasterState->Release();
	particleBlendState->Release();
	particleDepthState->Release();
	particleDebugRasterState->Release();
}
