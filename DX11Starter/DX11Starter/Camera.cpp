#include "Camera.h"
#include <stdio.h>

Camera::Camera(float x, float y, float z)
{
	// Set transformations
	cameraPosition = XMFLOAT3(x, y, z);;
	startPosition = cameraPosition;
	XMStoreFloat4(&cameraRotation, XMQuaternionIdentity());
	xRotation = 0;
	yRotation = 0;

	// Set matricies
	XMStoreFloat4x4(&viewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&projectionMatrix, XMMatrixIdentity());
}

Camera::~Camera() { }

void Camera::MoveRelative(float x, float y, float z)
{
	// Rotate the desired movement vector
	XMVECTOR dir = XMVector3Rotate(XMVectorSet(x, y, z, 0), XMLoadFloat4(&cameraRotation));

	// Move in that direction
	XMStoreFloat3(&cameraPosition, XMLoadFloat3(&cameraPosition) + dir);
}

void Camera::MoveAbsolute(float x, float y, float z)
{
	cameraPosition.x += x;
	cameraPosition.y += y;
	cameraPosition.z += z;
}

void Camera::Update(float deltaTime)
{
	// Calculate speed & speed up or down with SHIFT or TAB
	float speed = deltaTime * 3;
	if (GetAsyncKeyState(VK_SHIFT)) { speed *= 5; }
	if (GetAsyncKeyState(VK_TAB)) { speed *= 0.1f; }

	//CheckInput(speed);

	UpdateViewMatrix();
}

void Camera::Rotate(float xRot, float yRot)
{
	// Adjust the current rotation
	xRotation += xRot;
	yRotation += yRot;

	// Clamp the x between PI/2 and -PI/2
	xRotation = max(min(xRotation, XM_PIDIV2), -XM_PIDIV2);

	// Recreate the quaternion
	XMStoreFloat4(&cameraRotation, XMQuaternionRotationRollPitchYaw(xRotation, yRotation, 0));
}

void Camera::UpdateViewMatrix()
{
	// Rotate the standard "forward" matrix by our rotation
	// This gives us our "look direction"
	XMVECTOR dir = XMVector3Rotate(XMVectorSet(0, 0, 1, 0), XMLoadFloat4(&cameraRotation));

	XMMATRIX view = XMMatrixLookToLH(
		XMLoadFloat3(&cameraPosition),
		dir,
		XMVectorSet(0, 1, 0, 0));

	XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(view));
}

void Camera::UpdateProjectionMatrix(float aspectRatio)
{
	// Create the Projection matrix
	// - This should match the window's aspect ratio, and also update anytime
	//   the window resizes (which is already happening in OnResize() below)
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,		// Field of View Angle
		aspectRatio,		// Aspect ratio
		0.1f,						// Near clip plane distance
		100.0f);					// Far clip plane distance
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P)); // Transpose for HLSL!
}

void Camera::CheckInput(float speed)
{
	// WASD use relative move to move in the direction of the current rotation
	if (GetAsyncKeyState('W') & 0x8000) { MoveRelative(0, 0, speed); }
	if (GetAsyncKeyState('S') & 0x8000) { MoveRelative(0, 0, -speed); }
	if (GetAsyncKeyState('A') & 0x8000) { MoveRelative(-speed, 0, 0); }
	if (GetAsyncKeyState('D') & 0x8000) { MoveRelative(speed, 0, 0); }

	// Up & Down use absolute move
	if (GetAsyncKeyState(VK_CONTROL) & 0x8000) { MoveAbsolute(0, -speed, 0); }
	if (GetAsyncKeyState(VK_SPACE) & 0x8000) { MoveAbsolute(0, speed, 0); }

	// Check for reset
	if (GetAsyncKeyState('R') & 0x8000)
	{
		cameraPosition = startPosition;
		xRotation = 0;
		xRotation = 0;
		XMStoreFloat4(&cameraRotation, XMQuaternionIdentity());
	}
}
