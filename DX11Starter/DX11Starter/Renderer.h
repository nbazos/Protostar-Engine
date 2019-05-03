#pragma once

#include "DXCore.h"
#include "SimpleShader.h"
#include "Mesh.h"
#include "Lights.h"
#include "GameEntity.h"
#include "Camera.h"
#include "SceneManager.h"
#include "ParticleEmitter.h"
#include <DirectXMath.h>
#include <WICTextureLoader.h>
#include "DDSTextureLoader.h"

class Renderer {
public:
	Renderer() {};
	Renderer(ID3D11DeviceContext* context, ID3D11Device* device, unsigned int width, unsigned int height, ID3D11RenderTargetView* backBufferRTV, ID3D11DepthStencilView* depthStencilView, IDXGISwapChain* swapChain, Camera * cam, SceneManager* sceneMg);
	~Renderer() {};

	void Init();
	void Draw(float deltaTime, float totalTime);
	void ExitRenderer();

private: // Members
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;

	ID3D11DeviceContext* context;
	ID3D11Device* device;
	unsigned int width;
	unsigned int height;
	ID3D11RenderTargetView* backBufferRTV;
	ID3D11DepthStencilView* depthStencilView;
	IDXGISwapChain* swapChain;

	Material* material1;
	Material* material2;
	ID3D11ShaderResourceView* brickSRV;
	ID3D11ShaderResourceView* grassSRV;
	ID3D11SamplerState* sampler;

	// Lights
	DirectionalLight dirLight1;

	// Particle system members
	ID3D11ShaderResourceView* particleSRV;
	SimpleVertexShader* particleVS;
	SimplePixelShader* particlePS;
	ID3D11DepthStencilState* particleDepthState;
	ID3D11BlendState* particleBlendState;
	ID3D11RasterizerState* particleDebugRasterState;
	ParticleEmitter* emitter1;

	// Skybox members
	ID3D11ShaderResourceView* skySRV;
	SimpleVertexShader* skyVS;
	SimplePixelShader* skyPS;
	ID3D11RasterizerState* skyRasterState;
	ID3D11DepthStencilState* skyDepthState;

	// Game-related members
	Camera* camera;
	SceneManager* sceneManager;
	std::vector<Mesh*> meshes;
	std::vector<GameEntity>* gameEntities;

private: // Methods
	void LoadShaders();
	void CreateMaterials();
	void CreateBasicGeometry();
	void DrawSky();
};

