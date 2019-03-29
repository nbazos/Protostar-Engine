#pragma once

#include "DXCore.h"
#include "SimpleShader.h"
#include "Mesh.h"
#include "Lights.h"
#include "GameEntity.h"
#include "Camera.h"
#include <DirectXMath.h>
#include <WICTextureLoader.h>

class Render 
{
public:
	Render();
	Render(ID3D11DeviceContext*	context, ID3D11Device* device, float width, float height, ID3D11RenderTargetView* backBufferRTV, ID3D11DepthStencilView* depthStencilView, IDXGISwapChain* swapChain);
	~Render();

	void Init();
	void Update(float deltatime, float totalTime);
	void Draw(float deltaTime, float totalTime);
	void AdjustCameraProjectionOnResize();
	void RotateCamera(float x, float y);

private:

	void LoadShaders();
	void CreateMaterials();
	void CreateBasicGeometry();

	ID3D11DeviceContext* context;
	ID3D11Device* device;
	float width;
	float height;
	ID3D11RenderTargetView* backBufferRTV;
	ID3D11DepthStencilView* depthStencilView;
	IDXGISwapChain* swapChain;

	// Wrappers for DirectX shaders to provide simplified functionality
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;

	Material* material1;
	Material* material2;
	ID3D11ShaderResourceView* brickSRV;
	ID3D11ShaderResourceView* grassSRV;
	ID3D11SamplerState* sampler;

	std::vector<Mesh*> meshes;
	std::vector<GameEntity*> gameEntities;
	Camera* camera;
	DirectionalLight dirLight1;
	DirectionalLight dirLight2;
};

