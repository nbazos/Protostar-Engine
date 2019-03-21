#pragma once

#include <DirectXMath.h>

struct DirectionalLight
{
	DirectX::XMFLOAT4 AmbientColor;	    
	DirectX::XMFLOAT4 DiffuseColor;
	DirectX::XMFLOAT3 Direction;

	DirectionalLight()
	{
		AmbientColor.operator=(DirectX::XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f));
		DiffuseColor.operator=(DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f));
		Direction.operator=(DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f));
	}
};

struct DirectionalLight2
{
	DirectX::XMFLOAT4 AmbientColor;
	DirectX::XMFLOAT4 DiffuseColor;
	DirectX::XMFLOAT3 Direction;

	DirectionalLight2()
	{
		AmbientColor.operator=(DirectX::XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f));
		DiffuseColor.operator=(DirectX::XMFLOAT4(0.0f, 0.2f, 0.8f, 1.0f));
		Direction.operator=(DirectX::XMFLOAT3(1.0f, -1.0f, 0.0f));
	}
};