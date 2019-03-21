#pragma once

#include <DirectXMath.h>
#include <Windows.h>

using namespace DirectX;

class Camera
{
public:
	Camera();
	~Camera();

	// member variables
	XMFLOAT4X4 view4x4;
	XMFLOAT4X4 projection4x4;

	XMFLOAT4X4 GetViewMatrix4x4();
	XMFLOAT4X4 GetProjectionMatrix4x4();

	XMFLOAT3 positionFloat3;
	XMFLOAT3 directionFloat3;
	float xRot;
	float yRot;

	// methods
	void Update(float deltaTime);
	void GetKeyboardInput(float deltaTime, XMVECTOR dir);
	void UpdateMouseRotation(int xPixel, int yPixel);
	void UpdateProjectionMatrix4x4(unsigned int width, unsigned int height);
};

