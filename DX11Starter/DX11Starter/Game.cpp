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
	camera = 0;
	material1 = 0;
	material2 = 0;
	brickSRV = 0;
	grassSRV = 0;
	sampler = 0;

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
	delete camera;

	delete material1;
	delete material2;

	if (brickSRV != nullptr)
	{
		brickSRV->Release();
	}

	if (grassSRV != nullptr)
	{
		grassSRV->Release();
	}

	if (sampler != nullptr)
	{
		sampler->Release();
	}

	// Delete Meshes & GameEntities
	for (auto& m : meshes) delete m;
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
	CreateMaterials();

	// Create camera & initial projection matrix
	camera = new Camera(0.0f, 0.0f, -10.0f);
	camera->UpdateProjectionMatrix((float)width / height);
	
	// Define directional lights for the scene
	dirLight1 = { XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f), XMFLOAT4(0.75f, 0.75f, 0.75f, 1.0f), XMFLOAT3(1.0f, -1.0f, 0.0f) };
	dirLight2 = { XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f), XMFLOAT4(0.0f, 0.0f, 0.5f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f) };

	// Engine Subsystem & CommunicationI nitialization
	eventBus = EventBus();
	inputSystem = Input(&eventBus);
	sceneManager = SceneManager(&eventBus, camera);
	sceneManager.Init();
	CreateBasicGeometry();
	renderSystem = Render(context, device, width, height, backBufferRTV, depthStencilView, swapChain, sceneManager.GetSceneEntities(), camera);
	renderSystem.Init();

	// Tell the input assembler stage of the pipeline what kind of
	// geometric primitives (points, lines or triangles) we want to draw.  
	// Essentially: "What kind of shape should the GPU draw with our data?"
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Game::LoadShaders()
{
	vertexShader = new SimpleVertexShader(device, context);
	vertexShader->LoadShaderFile(L"VertexShader.cso");

	pixelShader = new SimplePixelShader(device, context);
	pixelShader->LoadShaderFile(L"PixelShader.cso");
}

void Game::CreateMaterials()
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
}

void Game::CreateBasicGeometry()
{
	Mesh* cubeMesh = new Mesh("../../Assets/Models/cube.obj", device);
	Mesh* coneMesh = new Mesh("../../Assets/Models/cone.obj", device);
	meshes.push_back(cubeMesh);
	meshes.push_back(coneMesh);

	// Create GameEntities that utilize the meshes
	GameEntity cone = GameEntity("Player", coneMesh, material1, context);
	GameEntity cube = GameEntity("Floor", cubeMesh, material1, context);

	// Set transformations
	cube.SetScale(5.0f, 1.0f, 1.0f);
	cube.SetPosition(0, -2.0f, 0);
	cube.SetRotation(0.0f, 0.0f, 0.0f);

	cone.SetScale(0.5f, 0.5f, 0.5f);
	cone.SetPosition(2, -1, 0);

	sceneManager.AddEntityToScene(cone);
	sceneManager.AddEntityToScene(cube);
}

// --------------------------------------------------------
// Handle resizing DirectX "stuff" to match the new window size.
// For instance, updating our projection matrix's aspect ratio.
// --------------------------------------------------------
void Game::OnResize()
{
	// Handle base-level DX resize stuff
	DXCore::OnResize();

	camera->UpdateProjectionMatrix((float)width / height);
}

// --------------------------------------------------------
// Update your game here - user input, move objects, AI, etc.
// --------------------------------------------------------
void Game::Update(float deltaTime, float totalTime)
{
	// Quit if the escape key is pressed
	if (GetAsyncKeyState(VK_ESCAPE))
		Quit();

	inputSystem.GetInput();
	sceneManager.Update(deltaTime, totalTime);
}

// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void Game::Draw(float deltaTime, float totalTime)
{
	pixelShader->SetData(
		"light1",
		&dirLight1,
		sizeof(DirectionalLight));
	pixelShader->SetData(
		"light2",
		&dirLight2,
		sizeof(DirectionalLight));

	renderSystem.Draw(deltaTime, totalTime);
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
	// Check left mouse button
	if (buttonState & 0x0001)
	{
		float xDiff = (x - prevMousePos.x) * 0.001f;
		float yDiff = (y - prevMousePos.y) * 0.001f;
		camera->Rotate(yDiff, xDiff);
	}

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

void Game::OnKeyDown(WPARAM keyCode)
{
	std::string key = vkToString(keyCode);
	std::cout << key << " was pressed" << std::endl;
}

void Game::OnKeyUp(WPARAM keyCode)
{
	std::string key = vkToString(keyCode);
	std::cout << key << " was released" << std::endl;
}

#define caseStringify(x) case x: return std::string(#x)

// Implementation from: http://cottonvibes.blogspot.com/2010/11/virtual-key-code-to-string.html
std::string Game::vkToString(int vk) 
{
	char c[2] = { 0 };
	if (vk >= '0' && vk <= '9') { c[0] = (char)vk; return std::string(c); }
	if (vk >= 'A' && vk <= 'Z') { c[0] = (char)vk; return std::string(c); }
	switch (vk) {
		// VK_0 - VK_9 are the same as ASCII '0' - '9' (0x30 - 0x39)
		// 0x40 : unassigned
		// VK_A - VK_Z are the same as ASCII 'A' - 'Z' (0x41 - 0x5A)
		caseStringify(VK_LBUTTON);
		caseStringify(VK_RBUTTON);
		caseStringify(VK_CANCEL);
		caseStringify(VK_MBUTTON);  // NOT contiguous with L & RBUTTON
		caseStringify(VK_XBUTTON1); // NOT contiguous with L & RBUTTON
		caseStringify(VK_XBUTTON2); // NOT contiguous with L & RBUTTON
		caseStringify(VK_BACK);
		caseStringify(VK_TAB);
		caseStringify(VK_CLEAR);
		caseStringify(VK_RETURN);
		caseStringify(VK_SHIFT);
		caseStringify(VK_CONTROL);
		caseStringify(VK_MENU);
		caseStringify(VK_PAUSE);
		caseStringify(VK_CAPITAL);
		caseStringify(VK_KANA);
		caseStringify(VK_JUNJA);
		caseStringify(VK_FINAL);
		caseStringify(VK_KANJI);
		caseStringify(VK_ESCAPE);
		caseStringify(VK_CONVERT);
		caseStringify(VK_NONCONVERT);
		caseStringify(VK_ACCEPT);
		caseStringify(VK_MODECHANGE);
		caseStringify(VK_SPACE);
		caseStringify(VK_PRIOR);
		caseStringify(VK_NEXT);
		caseStringify(VK_END);
		caseStringify(VK_HOME);
		caseStringify(VK_LEFT);
		caseStringify(VK_UP);
		caseStringify(VK_RIGHT);
		caseStringify(VK_DOWN);
		caseStringify(VK_SELECT);
		caseStringify(VK_PRINT);
		caseStringify(VK_EXECUTE);
		caseStringify(VK_SNAPSHOT);
		caseStringify(VK_INSERT);
		caseStringify(VK_DELETE);
		caseStringify(VK_HELP);
		caseStringify(VK_LWIN);
		caseStringify(VK_RWIN);
		caseStringify(VK_APPS);
		caseStringify(VK_SLEEP);
		caseStringify(VK_NUMPAD0);
		caseStringify(VK_NUMPAD1);
		caseStringify(VK_NUMPAD2);
		caseStringify(VK_NUMPAD3);
		caseStringify(VK_NUMPAD4);
		caseStringify(VK_NUMPAD5);
		caseStringify(VK_NUMPAD6);
		caseStringify(VK_NUMPAD7);
		caseStringify(VK_NUMPAD8);
		caseStringify(VK_NUMPAD9);
		caseStringify(VK_MULTIPLY);
		caseStringify(VK_ADD);
		caseStringify(VK_SEPARATOR);
		caseStringify(VK_SUBTRACT);
		caseStringify(VK_DECIMAL);
		caseStringify(VK_DIVIDE);
		caseStringify(VK_F1);
		caseStringify(VK_F2);
		caseStringify(VK_F3);
		caseStringify(VK_F4);
		caseStringify(VK_F5);
		caseStringify(VK_F6);
		caseStringify(VK_F7);
		caseStringify(VK_F8);
		caseStringify(VK_F9);
		caseStringify(VK_F10);
		caseStringify(VK_F11);
		caseStringify(VK_F12);
		caseStringify(VK_F13);
		caseStringify(VK_F14);
		caseStringify(VK_F15);
		caseStringify(VK_F16);
		caseStringify(VK_F17);
		caseStringify(VK_F18);
		caseStringify(VK_F19);
		caseStringify(VK_F20);
		caseStringify(VK_F21);
		caseStringify(VK_F22);
		caseStringify(VK_F23);
		caseStringify(VK_F24);
		caseStringify(VK_NUMLOCK);
		caseStringify(VK_SCROLL);
		caseStringify(VK_OEM_NEC_EQUAL);  // '=' key on numpad
		caseStringify(VK_OEM_FJ_MASSHOU); // 'Unregister word' key
		caseStringify(VK_OEM_FJ_TOUROKU); // 'Register word' key
		caseStringify(VK_OEM_FJ_LOYA);    // 'Left OYAYUBI' key
		caseStringify(VK_OEM_FJ_ROYA);    // 'Right OYAYUBI' key
		caseStringify(VK_LSHIFT);
		caseStringify(VK_RSHIFT);
		caseStringify(VK_LCONTROL);
		caseStringify(VK_RCONTROL);
		caseStringify(VK_LMENU);
		caseStringify(VK_RMENU);
		caseStringify(VK_BROWSER_BACK);
		caseStringify(VK_BROWSER_FORWARD);
		caseStringify(VK_BROWSER_REFRESH);
		caseStringify(VK_BROWSER_STOP);
		caseStringify(VK_BROWSER_SEARCH);
		caseStringify(VK_BROWSER_FAVORITES);
		caseStringify(VK_BROWSER_HOME);
		caseStringify(VK_VOLUME_MUTE);
		caseStringify(VK_VOLUME_DOWN);
		caseStringify(VK_VOLUME_UP);
		caseStringify(VK_MEDIA_NEXT_TRACK);
		caseStringify(VK_MEDIA_PREV_TRACK);
		caseStringify(VK_MEDIA_STOP);
		caseStringify(VK_MEDIA_PLAY_PAUSE);
		caseStringify(VK_LAUNCH_MAIL);
		caseStringify(VK_LAUNCH_MEDIA_SELECT);
		caseStringify(VK_LAUNCH_APP1);
		caseStringify(VK_LAUNCH_APP2);
		caseStringify(VK_OEM_1);      // ';:' for US
		caseStringify(VK_OEM_PLUS);   // '+' any country
		caseStringify(VK_OEM_COMMA);  // ',' any country
		caseStringify(VK_OEM_MINUS);  // '-' any country
		caseStringify(VK_OEM_PERIOD); // '.' any country
		caseStringify(VK_OEM_2);  // '/?' for US
		caseStringify(VK_OEM_3);  // '`~' for US
		caseStringify(VK_OEM_4);  //  '[{' for US
		caseStringify(VK_OEM_5);  //  '\|' for US
		caseStringify(VK_OEM_6);  //  ']}' for US
		caseStringify(VK_OEM_7);  //  ''"' for US
		caseStringify(VK_OEM_8);
		caseStringify(VK_OEM_AX);   //  'AX' key on Japanese AX kbd
		caseStringify(VK_OEM_102);  //  "<>" or "\|" on RT 102-key kbd.
		caseStringify(VK_ICO_HELP); //  Help key on ICO
		caseStringify(VK_ICO_00);   //  00 key on ICO
		caseStringify(VK_PROCESSKEY);
		caseStringify(VK_ICO_CLEAR);
		caseStringify(VK_PACKET);
	}
	c[0] = (char)vk;
	return std::string(c);
}