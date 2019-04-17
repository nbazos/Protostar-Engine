#pragma once

#include <DirectXMath.h>
#include <Windows.h>

// For the DirectX Math library
using namespace DirectX;

class Camera {
public:
	Camera(float x, float y, float z);
	~Camera();

	void MoveRelative(float x, float y, float z); // Moves the camera relative to its orientation
	void MoveAbsolute(float x, float y, float z); // Moves the camera in world space
	void Rotate(float xRot, float yRot);

	void Update(float deltaTime);
	void UpdateViewMatrix();
	void UpdateProjectionMatrix(float aspectRatio);

	XMFLOAT4X4 GetViewMatrix() { return viewMatrix; };
	XMFLOAT4X4 GetProjectionMatrix() { return projectionMatrix; };

	void CheckInput(float moveSpeed);

	XMFLOAT3 cameraPosition;

private:
	XMFLOAT4X4 viewMatrix;
	XMFLOAT4X4 projectionMatrix;

	XMFLOAT3 startPosition;
	
	XMFLOAT4 cameraRotation;
	float xRotation;
	float yRotation;

	
};