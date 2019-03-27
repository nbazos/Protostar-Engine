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
	Render();
	~Render();

	void Init();

	void LoadShaders();
	void CreateMaterials();
	void CreateBasicGeometry();

	void AdjustCameraProjectionOnResize();

	void Update();

	void Draw(float deltaTime, float totalTime);

	//// Wrappers for DirectX shaders to provide simplified functionality
	//SimpleVertexShader* vertexShader;
	//SimplePixelShader* pixelShader;

	//Material* material1;
	//Material* material2;
	//ID3D11ShaderResourceView* brickSRV;
	//ID3D11ShaderResourceView* grassSRV;
	//ID3D11SamplerState* sampler;

	/*std::vector<Mesh*> meshes;
	std::vector<GameEntity*> gameEntities;
	Camera* camera;
	DirectionalLight dirLight1;
	DirectionalLight dirLight2;*/
};

