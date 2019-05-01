#include "Render.h"
#include "WICTextureLoader.h"

Render::Render(ID3D11DeviceContext*	ctx, ID3D11Device* d, unsigned int w, unsigned int h, ID3D11RenderTargetView* bBRTV, ID3D11DepthStencilView* dSV, IDXGISwapChain* sC, std::vector<GameEntity> * entities, Camera * cam)
{
	context = ctx;
	device = d;
	width = w;
	height = h;
	backBufferRTV = bBRTV;
	depthStencilView = dSV;
	swapChain = sC;
	gameEntities = entities;
	camera = cam;
}

void Render::Init()
{
	/// SLATE
	//eventBus->Subscribe(this, &Render::ShowSlate);

	//CreateWICTextureFromFile(
	//	device,					// The Direct3D device for resource creation
	//	context,				// Rendering context (this will auto-generate mip maps!!!)
	//	L"../../Assets/Textures/checkerboard.png",	// Path to the file ("L" means wide characters)
	//	0,						// Texture ref?  No thanks!  (0 means we don't want an extra ref)
	//	&slateSRV);				// Actual SRV for use with shaders

	//	spriteBatch = std::make_unique<SpriteBatch>(context);
}

void Render::Draw(float deltaTime, float totalTime)
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

	for (int i = 0; i < (*gameEntities).size(); i++)
	{
		(*gameEntities)[i].Draw(camera->GetViewMatrix(), camera->GetProjectionMatrix());
	}

	/*if (slateDisplaying)
	{
		spriteBatch->Begin();
		spriteBatch->Draw(slateSRV, XMFLOAT2(100, 100));
		spriteBatch->End();
	}*/

	// Present the back buffer to the user
	//  - Puts the final frame we're drawing into the window so the user can see it
	//  - Do this exactly ONCE PER FRAME (always at the very end of the frame)
	swapChain->Present(0, 0);
}

//void Render::ShowSlate(InputSlate * inputEvent)
//{
//	if (slateDisplaying)
//	{
//		slateDisplaying = false;
//	}
//	else
//	{
//		slateDisplaying = true;
//	}
//}

