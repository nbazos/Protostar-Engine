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
	void SetPosition(float x, float y, float z);

	void Update(float deltaTime);
	void UpdateViewMatrix();
	void UpdateProjectionMatrix(float aspectRatio);

	XMFLOAT4X4 GetViewMatrix() { return viewMatrix; };
	XMFLOAT4X4 GetProjectionMatrix() { return projectionMatrix; };
	XMFLOAT3 GetPosition() { return cameraPosition; };

	void CheckInput(float moveSpeed);

private:
	XMFLOAT4X4 viewMatrix;
	XMFLOAT4X4 projectionMatrix;

	XMFLOAT3 startPosition;
	XMFLOAT3 cameraPosition;

	XMFLOAT4 cameraRotation;
	float xRotation;
	float yRotation;

	
};