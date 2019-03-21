#include "Game.h"
#include "Vertex.h"

// For the DirectX Math library
using namespace DirectX;

// --------------------------------------------------------
// Constructor
//
// DXCore (base class) constructor will set up underlying fields.
// DirectX itself, and our window, are not ready yet!
//
// hInstance - the application's OS-level handle (unique ID)
// --------------------------------------------------------
Game::Game(HINSTANCE hInstance)
	: DXCore(
		hInstance,		// The application's handle
		"DirectX Game",	   	// Text for the window's title bar
		1280,			// Width of the window's client area
		720,			// Height of the window's client area
		true)			// Show extra stats (fps) in title bar?
{
	// Initialize fields
	vertexShader = 0;
	pixelShader = 0;

	// A4
	camera = Camera();
	camera.UpdateProjectionMatrix4x4(1280, 720);

#if defined(DEBUG) || defined(_DEBUG)
	// Do we want a console window?  Probably only in debug mode
	CreateConsoleWindow(500, 120, 32, 120);
	printf("Console window created successfully.  Feel free to printf() here.\n");
#endif
	
}

// --------------------------------------------------------
// Destructor - Clean up anything our game has created:
//  - Release all DirectX objects created here
//  - Delete any objects to prevent memory leaks
// --------------------------------------------------------
Game::~Game()
{
	// Delete our simple shader objects, which
	// will clean up their own internal DirectX stuff
	delete vertexShader;
	delete pixelShader;

	// A2 - Delete Mesh Objects
	delete meshObj1;
	delete meshObj2;
	delete meshObj3;

	// A4 & A6 - Delete Material
	delete stoneWallMat;
	delete rockMat;
	delete brickMat;

	// A6
	samplerStatePtr->Release();
	rockSurfaceSRVPtr->Release();
	stoneWallSurfaceSRVPtr->Release();
	brickSurfaceSRVPtr->Release();
}

// --------------------------------------------------------
// Called once per program, after DirectX and the window
// are initialized but before the game loop.
// --------------------------------------------------------
void Game::Init()
{
	// Helper methods for loading shaders, creating some basic
	// geometry to draw and some simple camera matrices.
	//  - You'll be expanding and/or replacing these later
	LoadShaders();
	CreateMatrices();
	CreateBasicGeometry();

	// A6

	// Load Textures
	rockSurfaceSRVPtr = nullptr;
	CreateWICTextureFromFile(device, context, L"../../Assets/Textures/rocksurface.tif", 0, &rockSurfaceSRVPtr);

	stoneWallSurfaceSRVPtr = nullptr;
	CreateWICTextureFromFile(device, context, L"../../Assets/Textures/stonewall.tif", 0, &stoneWallSurfaceSRVPtr);

	brickSurfaceSRVPtr = nullptr;
	CreateWICTextureFromFile(device, context, L"../../Assets/Textures/brick.png", 0, &brickSurfaceSRVPtr);

	// Sampler State
	D3D11_SAMPLER_DESC samplerState = {};
	samplerState.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerState.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerState.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerState.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerState.MaxLOD = D3D11_FLOAT32_MAX;

	device->CreateSamplerState(&samplerState, &samplerStatePtr);

	// A3 & A4 & A6

	// Define the materials and meshes of the in-game entities
	stoneWallMat = new Material(vertexShader, pixelShader, stoneWallSurfaceSRVPtr, samplerStatePtr);

	entities.push_back(GameEntity(meshObj1, stoneWallMat));
	entities.push_back(GameEntity(meshObj1, stoneWallMat));
	entities.push_back(GameEntity(meshObj1, stoneWallMat));

	rockMat = new Material(vertexShader, pixelShader, rockSurfaceSRVPtr, samplerStatePtr);

	entities.push_back(GameEntity(meshObj2, rockMat));
	entities.push_back(GameEntity(meshObj2, rockMat));
	entities.push_back(GameEntity(meshObj2, rockMat));

	brickMat = new Material(vertexShader, pixelShader, brickSurfaceSRVPtr, samplerStatePtr);

	entities.push_back(GameEntity(meshObj3, brickMat));
	entities.push_back(GameEntity(meshObj3, brickMat));
	entities.push_back(GameEntity(meshObj3, brickMat));

	// A5
	dirLight = DirectionalLight();
	dirLight2 = DirectionalLight2();

	// Event Bus
	eventBus = EventBus();
	// Game Systems
	playerEntitySystem = PlayerEntity(&eventBus);
	playerEntitySystem.init();
	inputSystem = Input(&eventBus);
	inputSystem.init();

	// Tell the input assembler stage of the pipeline what kind of
	// geometric primitives (points, lines or triangles) we want to draw.  
	// Essentially: "What kind of shape should the GPU draw with our data?"
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

// --------------------------------------------------------
// Loads shaders from compiled shader object (.cso) files using
// my SimpleShader wrapper for DirectX shader manipulation.
// - SimpleShader provides helpful methods for sending
//   data to individual variables on the GPU
// --------------------------------------------------------
void Game::LoadShaders()
{
	vertexShader = new SimpleVertexShader(device, context);
	vertexShader->LoadShaderFile(L"VertexShader.cso");

	pixelShader = new SimplePixelShader(device, context);
	pixelShader->LoadShaderFile(L"PixelShader.cso");
}



// --------------------------------------------------------
// Initializes the matrices necessary to represent our geometry's 
// transformations and our 3D camera
// --------------------------------------------------------
void Game::CreateMatrices()
{
	// Set up world matrix
	// - In an actual game, each object will need one of these and they should
	//    update when/if the object moves (every frame)
	// - You'll notice a "transpose" happening below, which is redundant for
	//    an identity matrix.  This is just to show that HLSL expects a different
	//    matrix (column major vs row major) than the DirectX Math library
	XMMATRIX W = XMMatrixIdentity();
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(W)); // Transpose for HLSL!

	// Create the View matrix
	// - In an actual game, recreate this matrix every time the camera 
	//    moves (potentially every frame)
	// - We're using the LOOK TO function, which takes the position of the
	//    camera and the direction vector along which to look (as well as "up")
	// - Another option is the LOOK AT function, to look towards a specific
	//    point in 3D space
	XMVECTOR pos = XMVectorSet(0, 0, -5, 0);
	XMVECTOR dir = XMVectorSet(0, 0, 1, 0);
	XMVECTOR up = XMVectorSet(0, 1, 0, 0);
	XMMATRIX V = XMMatrixLookToLH(
		pos,     // The position of the "camera"
		dir,     // Direction the camera is looking
		up);     // "Up" direction in 3D space (prevents roll)
	XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(V)); // Transpose for HLSL!

	// Create the Projection matrix
	// - This should match the window's aspect ratio, and also update anytime
	//    the window resizes (which is already happening in OnResize() below)
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,		// Field of View Angle
		(float)width / height,		// Aspect ratio
		0.1f,						// Near clip plane distance
		100.0f);					// Far clip plane distance
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P)); // Transpose for HLSL!
}


// --------------------------------------------------------
// Creates the geometry we're going to draw - a single triangle for now
// --------------------------------------------------------
void Game::CreateBasicGeometry()
{
	// Create some temporary variables to represent colors
	// - Not necessary, just makes things more readable
	XMFLOAT4 red = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4 green = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	XMFLOAT4 blue = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);

	// Set up the vertices of the triangle we would like to draw
	// - We're going to copy this array, exactly as it exists in memory
	//    over to a DirectX-controlled data structure (the vertex buffer)
	Vertex obj1Vertices[] =
	{
		{ XMFLOAT3(+2.0f, +1.0f, +0.0f), XMFLOAT3(0,0,-1), XMFLOAT2(0,0) },
		{ XMFLOAT3(+2.5f, +0.0f, +0.0f), XMFLOAT3(0,0,-1), XMFLOAT2(0,0) },
		{ XMFLOAT3(+1.5f, +0.0f, +0.0f), XMFLOAT3(0,0,-1), XMFLOAT2(0,0) },
	};

	// Set up the indices, which tell us which vertices to use and in which order
	// - This is somewhat redundant for just 3 vertices (it's a simple example)
	// - Indices are technically not required if the vertices are in the buffer 
	//    in the correct order and each one will be used exactly once
	// - But just to see how it's done...

	// A5 - New Geometry

	meshObj1 = new Mesh("../../Assets/Models/cube.obj", device);

	meshObj2 = new Mesh("../../Assets/Models/helix.obj", device);

	meshObj3 = new Mesh("../../Assets/Models/sphere.obj", device);
}


// --------------------------------------------------------
// Handle resizing DirectX "stuff" to match the new window size.
// For instance, updating our projection matrix's aspect ratio.
// --------------------------------------------------------
void Game::OnResize()
{
	// Handle base-level DX resize stuff
	DXCore::OnResize();

	// Update our projection matrix since the window size changed
	/*
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,	// Field of View Angle
		(float)width / height,	// Aspect ratio
		0.1f,				  	// Near clip plane distance
		100.0f);			  	// Far clip plane distance
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P)); // Transpose for HLSL!
	*/

	camera.UpdateProjectionMatrix4x4(width, height);

}

// --------------------------------------------------------
// Update your game here - user input, move objects, AI, etc.
// --------------------------------------------------------
void Game::Update(float deltaTime, float totalTime)
{
	// Quit if the escape key is pressed
	if (GetAsyncKeyState(VK_ESCAPE))
		Quit();

	// Event-Messaging System Test
	inputSystem.getInput();

	// A4
	camera.Update(deltaTime);

	// A3

	// Create sinTiem variable that increases and decreases
	float sinTime = (sin(totalTime * 10) + 2.0f) / 10.0f;

	// Do transformation work for each entity

	// Entity 1
	entities[0].SetTranslationMatrix4X4(XMMatrixTranslation(-1, -1, 0));
	XMVECTOR rotQuat = XMQuaternionRotationRollPitchYaw(0, 0, totalTime);
	entities[0].SetRotMatrix4X4(XMMatrixRotationQuaternion(rotQuat));
	entities[0].SetScaleMatrix4X4(XMMatrixScaling(sinTime, sinTime, sinTime));

	entities[0].CalculateWorldMatrix();

	// Entity 2
	entities[1].SetTranslationMatrix4X4(XMMatrixTranslation(1, -1, 0));
	entities[1].SetRotMatrix4X4(XMMatrixRotationQuaternion(rotQuat));
	entities[1].SetScaleMatrix4X4(XMMatrixScaling(sinTime, sinTime, sinTime));

	entities[1].CalculateWorldMatrix();

	// Entity 3
	entities[2].SetTranslationMatrix4X4(XMMatrixTranslation(0, -1, 0));
	entities[2].SetRotMatrix4X4(XMMatrixRotationQuaternion(rotQuat));
	entities[2].SetScaleMatrix4X4(XMMatrixScaling(sinTime, sinTime, sinTime));

	entities[2].CalculateWorldMatrix();

	// Entity 4
	entities[3].SetTranslationMatrix4X4(XMMatrixTranslation(-1, 1, 0));
	entities[3].SetRotMatrix4X4(XMMatrixRotationQuaternion(rotQuat));
	entities[3].SetScaleMatrix4X4(XMMatrixScaling(sinTime, sinTime, sinTime));

	entities[3].CalculateWorldMatrix();

	// Entity 5
	entities[4].SetTranslationMatrix4X4(XMMatrixTranslation(1, 1, 0));
	entities[4].SetRotMatrix4X4(XMMatrixRotationQuaternion(rotQuat));
	entities[4].SetScaleMatrix4X4(XMMatrixScaling(sinTime, sinTime, sinTime));

	entities[4].CalculateWorldMatrix();

	// Entity 6 
	entities[5].SetTranslationMatrix4X4(XMMatrixTranslation(0, 1, 0));
	entities[5].SetRotMatrix4X4(XMMatrixRotationQuaternion(rotQuat));
	entities[5].SetScaleMatrix4X4(XMMatrixScaling(sinTime, sinTime, sinTime));

	entities[5].CalculateWorldMatrix();

	// Entity 7 
	entities[6].SetTranslationMatrix4X4(XMMatrixTranslation(-1, 0, 0));
	entities[6].SetRotMatrix4X4(XMMatrixRotationQuaternion(rotQuat));
	entities[6].SetScaleMatrix4X4(XMMatrixScaling(sinTime, sinTime, sinTime));

	entities[6].CalculateWorldMatrix();

	// Entity 8 
	entities[7].SetTranslationMatrix4X4(XMMatrixTranslation(1, 0, 0));
	entities[7].SetRotMatrix4X4(XMMatrixRotationQuaternion(rotQuat));
	entities[7].SetScaleMatrix4X4(XMMatrixScaling(sinTime, sinTime, sinTime));

	entities[7].CalculateWorldMatrix();

	// Entity 9 
	entities[8].SetTranslationMatrix4X4(XMMatrixTranslation(0, 0, 0));
	entities[8].SetRotMatrix4X4(XMMatrixRotationQuaternion(rotQuat));
	entities[8].SetScaleMatrix4X4(XMMatrixScaling(sinTime, sinTime, sinTime));

	entities[8].CalculateWorldMatrix();
}

// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void Game::Draw(float deltaTime, float totalTime)
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

	// Send data to shader variables
	//  - Do this ONCE PER OBJECT you're drawing
	//  - This is actually a complex process of copying data to a local buffer
	//    and then copying that entire buffer to the GPU.  
	//  - The "SimpleShader" class handles all of that for you.
	
	// A3

	// Loop through all entities and draw 
	for each (GameEntity entity in entities)
	{
		// Set entity specific data
		entity.PrepareMaterial(camera.GetViewMatrix4x4(), camera.GetProjectionMatrix4x4());

		// Set Light Data in PixelShader
		pixelShader->SetData("light", &dirLight, sizeof(DirectionalLight));
		pixelShader->SetData("light2", &dirLight2, sizeof(DirectionalLight2));

		// Once you've set all of the data you care to change for
		// the next draw call, you need to actually send it to the GPU
		//  - If you skip this, the "SetMatrix" calls above won't make it to the GPU!
		vertexShader->CopyAllBufferData();
		pixelShader->CopyAllBufferData();

		// Set the vertex and pixel shaders to use for the next Draw() command
		//  - These don't technically need to be set every frame...YET
		//  - Once you start applying different shaders to different objects,
		//    you'll need to swap the current shaders before each draw
		vertexShader->SetShader();
		pixelShader->SetShader();

		UINT stride = sizeof(Vertex);
		UINT offset = 0;

		ID3D11Buffer* entityVertexBuffer = entity.meshPtr->GetVertexBuffer();

		context->IASetVertexBuffers(0, 1, &entityVertexBuffer, &stride, &offset);
		context->IASetIndexBuffer(entity.meshPtr->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

		context->DrawIndexed(
			entity.meshPtr->GetIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
			0,     // Offset to the first index we want to use
			0);    // Offset to add to each index when looking up verticesv
	}

	// Present the back buffer to the user
	//  - Puts the final frame we're drawing into the window so the user can see it
	//  - Do this exactly ONCE PER FRAME (always at the very end of the frame)
	swapChain->Present(0, 0);
}


#pragma region Mouse Input

// --------------------------------------------------------
// Helper method for mouse clicking.  We get this information
// from the OS-level messages anyway, so these helpers have
// been created to provide basic mouse input if you want it.
// --------------------------------------------------------
void Game::OnMouseDown(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...

	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;

	// Caputure the mouse so we keep getting mouse move
	// events even if the mouse leaves the window.  we'll be
	// releasing the capture once a mouse button is released
	SetCapture(hWnd);
}

// --------------------------------------------------------
// Helper method for mouse release
// --------------------------------------------------------
void Game::OnMouseUp(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...

	// We don't care about the tracking the cursor outside
	// the window anymore (we're not dragging if the mouse is up)
	ReleaseCapture();
}

// --------------------------------------------------------
// Helper method for mouse movement.  We only get this message
// if the mouse is currently over the window, or if we're 
// currently capturing the mouse.
// --------------------------------------------------------
void Game::OnMouseMove(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...

	// A4
	camera.UpdateMouseRotation(x, y);

	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;
}

// --------------------------------------------------------
// Helper method for mouse wheel scrolling.  
// WheelDelta may be positive or negative, depending 
// on the direction of the scroll
// --------------------------------------------------------
void Game::OnMouseWheel(float wheelDelta, int x, int y)
{
	// Add any custom code here...
}
#pragma endregion