#pragma once

#include "DXCore.h"
#include "SimpleShader.h"
#include <DirectXMath.h>
#include "GameEntity.h"
#include "Camera.h"
#include "Lights.h"
#include "WICTextureLoader.h"
#include "EventBus.h"
#include "PlayerEntity.h"
#include "Input.h"

class Game 
	: public DXCore
{

public:
	Game(HINSTANCE hInstance);
	~Game();

	// Overridden setup and game loop methods, which
	// will be called automatically
	void Init();
	void OnResize();
	void Update(float deltaTime, float totalTime);
	void Draw(float deltaTime, float totalTime);

	// Overridden mouse input helper methods
	void OnMouseDown (WPARAM buttonState, int x, int y);
	void OnMouseUp	 (WPARAM buttonState, int x, int y);
	void OnMouseMove (WPARAM buttonState, int x, int y);
	void OnMouseWheel(float wheelDelta,   int x, int y);
private:

	// Initialization helper methods - feel free to customize, combine, etc.
	void LoadShaders(); 
	void CreateMatrices();
	void CreateBasicGeometry();

	// Wrappers for DirectX shaders to provide simplified functionality
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;

	// The matrices to go from model space to screen space
	XMFLOAT4X4 worldMatrix;
	XMFLOAT4X4 viewMatrix;
	XMFLOAT4X4 projectionMatrix;

	// Keeps track of the old mouse position.  Useful for 
	// determining how far the mouse moved in a single frame.
	POINT prevMousePos;

	// A2 - 3 private mesh variables
	Mesh* meshObj1;
	Mesh* meshObj2;
	Mesh* meshObj3;

	// A3 - vector of GameEntity objects
	std::vector<GameEntity> entities;

	// A4
	Camera camera;
	Material* stoneWallMat;
	Material* rockMat;
	Material* brickMat;

	// A5
	DirectionalLight dirLight;
	DirectionalLight2 dirLight2;

	// A6
	ID3D11SamplerState* samplerStatePtr;

	ID3D11ShaderResourceView* rockSurfaceSRVPtr;
	ID3D11ShaderResourceView* stoneWallSurfaceSRVPtr;
	ID3D11ShaderResourceView* brickSurfaceSRVPtr;

	// Event Bus
	EventBus eventBus;

	// Game Systems
	PlayerEntity playerEntitySystem;
	Input inputSystem;
};

