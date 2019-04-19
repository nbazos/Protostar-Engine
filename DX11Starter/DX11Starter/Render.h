#pragma once

#include "DXCore.h"
#include "SimpleShader.h"
#include "Mesh.h"
#include "Lights.h"
#include "GameEntity.h"
#include "Camera.h"
#include <DirectXMath.h>


class Render 
{
public:
	Render() {};
	Render(ID3D11DeviceContext*	context, ID3D11Device* device, unsigned int width, unsigned int height, ID3D11RenderTargetView* backBufferRTV, ID3D11DepthStencilView* depthStencilView, IDXGISwapChain* swapChain, std::vector<GameEntity> * entities, Camera * cam);
	~Render() {};

	void Init();
	void Draw(float deltaTime, float totalTime);
	void AdjustCameraProjectionOnResize();
	void RotateCamera(float x, float y);

private:
	ID3D11DeviceContext* context;
	ID3D11Device* device;
	unsigned int width;
	unsigned int height;
	ID3D11RenderTargetView* backBufferRTV;
	ID3D11DepthStencilView* depthStencilView;
	IDXGISwapChain* swapChain;

	Camera* camera;
	std::vector<GameEntity>* gameEntities;
};

