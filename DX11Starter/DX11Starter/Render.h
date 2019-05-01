#pragma once

#include "DXCore.h"
#include "SimpleShader.h"
#include "Mesh.h"
#include "Lights.h"
#include "GameEntity.h"
#include "Camera.h"
#include <DirectXMath.h>
#include "System.h"
#include "Events.h"
#include "SpriteBatch.h"

class Render /// SLATE /*: public System*/
{
public:
	Render() {};
	Render(/*EventBus * eventBusPtr,*/ ID3D11DeviceContext*	context, ID3D11Device* device, unsigned int width, unsigned int height,
		ID3D11RenderTargetView* backBufferRTV, ID3D11DepthStencilView* depthStencilView, IDXGISwapChain* swapChain,
		std::vector<GameEntity> * entities, Camera * cam); /*: System(eventBusPtr), context{ context },
		device{ device }, width{ width }, height{ height }, backBufferRTV{ backBufferRTV }, depthStencilView{ depthStencilView },
		swapChain{ swapChain }, gameEntities{ entities }, camera{ cam } {};*/
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

	/// SLATE
	//bool slateDisplaying;
	//void ShowSlate(InputSlate * inputEvent);

	//ID3D11ShaderResourceView* slateSRV;
	//std::unique_ptr<SpriteBatch> spriteBatch;

};

