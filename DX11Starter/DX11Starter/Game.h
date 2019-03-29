#pragma once

#include "EventBus.h"
#include "Input.h"
#include "PlayerEntity.h"
#include "Render.h"


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

	// Overridden mouse input helper methods
	void OnMouseDown (WPARAM buttonState, int x, int y);
	void OnMouseUp	 (WPARAM buttonState, int x, int y);
	void OnMouseMove (WPARAM buttonState, int x, int y);
	void OnMouseWheel(float wheelDelta,   int x, int y);
private:

	//// Initialization helper methods - feel free to customize, combine, etc.
	//void LoadShaders();
	//void CreateMaterials();
	//void CreateBasicGeometry();

	//// Wrappers for DirectX shaders to provide simplified functionality
	//SimpleVertexShader* vertexShader;
	//SimplePixelShader* pixelShader;

	//Material* material1;
	//Material* material2;
	//ID3D11ShaderResourceView* brickSRV;
	//ID3D11ShaderResourceView* grassSRV;
	//ID3D11SamplerState* sampler;

	// Keeps track of the old mouse position.  Useful for 
	// determining how far the mouse moved in a single frame.
	POINT prevMousePos;

	//std::vector<Mesh*> meshes;
	//std::vector<GameEntity*> gameEntities;
	//Camera* camera;
	//DirectionalLight dirLight1;
	//DirectionalLight dirLight2;

	// Engine systems
	EventBus eventBus;
	Input inputSystem;
	PlayerEntity playerEntitySystem;
	Render renderSystem;
};

