#include "Camera.h"



Camera::Camera()
{
	XMMATRIX W = XMMatrixIdentity();

	XMStoreFloat4x4(&view4x4, XMMatrixTranspose(W));

	XMStoreFloat4x4(&projection4x4, XMMatrixTranspose(W));

	positionFloat3 = {0, 0, -5.0f};
	directionFloat3 = {0, 0, 1.0f};

	yRot = 0;
	xRot = 0;
}

Camera::~Camera()
{
}

XMFLOAT4X4 Camera::GetViewMatrix4x4()
{
	return view4x4;
}

XMFLOAT4X4 Camera::GetProjectionMatrix4x4()
{
	return projection4x4;
}

void Camera::Update(float deltaTime)
{
	// calculate view matrix
	XMVECTOR rotQuat = XMQuaternionRotationRollPitchYaw(xRot, yRot, 0.0f);

	XMVECTOR direction = XMVector3Rotate(XMLoadFloat3(&directionFloat3), rotQuat);

	XMMATRIX view = XMMatrixLookToLH(XMLoadFloat3(&positionFloat3), direction, XMVectorSet(0, 1, 0, 0));

	XMStoreFloat4x4(&view4x4, XMMatrixTranspose(view));

	// get input
	GetKeyboardInput(deltaTime, direction);
}

void Camera::GetKeyboardInput(float deltaTime, XMVECTOR dir)
{
	// Forward
	if (GetAsyncKeyState('W') & 0x8000)
	{
		// Position + (Direction * dT)
		XMStoreFloat3(&positionFloat3, XMVectorAdd(XMLoadFloat3(&positionFloat3), XMVectorScale(dir, deltaTime)));
	}

	// Back
	if (GetAsyncKeyState('S') & 0x8000)
	{
		// Position + -(Direction * dT)
		XMStoreFloat3(&positionFloat3, XMVectorAdd(XMLoadFloat3(&positionFloat3), XMVectorNegate(XMVectorScale(dir, deltaTime))));
	}

	// Left
	if (GetAsyncKeyState('A') & 0x8000)
	{
		// Position + (Cross(direction,up) * dT)
		XMStoreFloat3(&positionFloat3, XMVectorAdd(XMLoadFloat3(&positionFloat3), XMVectorScale(XMVector3Cross(dir, XMVectorSet(0,1,0,0)), deltaTime)));
	}

	// Right
	if (GetAsyncKeyState('D') & 0x8000)
	{
		// Position + -(Cross(direction,up) * dT)
		XMStoreFloat3(&positionFloat3, XMVectorAdd(XMLoadFloat3(&positionFloat3), XMVectorNegate(XMVectorScale(XMVector3Cross(dir, XMVectorSet(0, 1, 0, 0)), deltaTime))));
	}

	// Up
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		// Position + 1 * dT
		positionFloat3.y += 1 * deltaTime;
	}

	// Down
	if (GetAsyncKeyState('X') & 0x8000)
	{
		// Position + -1 * dT
		positionFloat3.y += -1 * deltaTime;
	}
}

void Camera::UpdateMouseRotation(int xPixel, int yPixel)
{
	// scale down the pixel values as the they will be interpreted as radians
	xRot = yPixel * 0.001f;
	yRot = xPixel * 0.001f;
}

void Camera::UpdateProjectionMatrix4x4(unsigned int width, unsigned int height)
{
	// Update our projection matrix since the window size changed
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,	// Field of View Angle
		(float)width / height,	// Aspect ratio
		0.1f,				  	// Near clip plane distance
		100.0f);			  	// Far clip plane distance
	XMStoreFloat4x4(&projection4x4, XMMatrixTranspose(P)); // Transpose for HLSL!
}
