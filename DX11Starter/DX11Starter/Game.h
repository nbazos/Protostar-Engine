#pragma once

#include "EventBus.h"
#include "Input.h"
#include "SceneManager.h"
#include "Render.h"

#include <WICTextureLoader.h>

class Game : public DXCore {
public:
	Game(HINSTANCE hInstance);
	~Game();

	// Overridden setup and game loop methods, which
	// will be called automatically
	void Init();
	void OnResize();
	void Update(float deltaTime, float totalTime);
	void Draw(float deltaTime, float totalTime);

	// 
	void CreateBasicGeometry();
	void CreateMaterials();
	void LoadShaders();

	// Overridden mouse input helper methods
	void OnMouseDown (WPARAM buttonState, int x, int y);
	void OnMouseUp	 (WPARAM buttonState, int x, int y);
	void OnMouseMove (WPARAM buttonState, int x, int y);
	void OnMouseWheel(float wheelDelta,   int x, int y);
private:
	// Keeps track of the old mouse position.  Useful for 
	// determining how far the mouse moved in a single frame.
	POINT prevMousePos;

	// Wrappers for DirectX shaders to provide simplified functionality
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;

	Material* material1;
	Material* material2;
	ID3D11ShaderResourceView* brickSRV;
	ID3D11ShaderResourceView* grassSRV;
	ID3D11SamplerState* sampler;

	std::vector<Mesh*> meshes;
	Camera* camera;

	DirectionalLight dirLight1;
	DirectionalLight dirLight2;
	
	// Engine systems
	EventBus eventBus;
	Input* inputSystem;
	SceneManager sceneManager;
	Render renderSystem;
};

