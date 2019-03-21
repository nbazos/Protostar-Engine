#include "GameEntity.h"

GameEntity::GameEntity(Mesh* meshPTr, Material* materialPtr)
{
	this->meshPtr = meshPTr;
	this->materialPtr = materialPtr;

	XMMATRIX W = XMMatrixIdentity();

	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(W)); // Transpose for HLSL!;

	XMStoreFloat4x4(&trans4X4, XMMatrixTranspose(W)); // Transpose for HLSL!;

	XMStoreFloat4x4(&scale4X4, XMMatrixTranspose(W)); // Transpose for HLSL!;

	XMStoreFloat4x4(&rot4X4, XMMatrixTranspose(W)); // Transpose for HLSL!;
}

GameEntity::GameEntity()
{
	this->meshPtr = nullptr;
	this->materialPtr = nullptr;

	XMMATRIX W = XMMatrixIdentity();

	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(W)); // Transpose for HLSL!;

	XMStoreFloat4x4(&trans4X4, XMMatrixTranspose(W)); // Transpose for HLSL!;

	XMStoreFloat4x4(&scale4X4, XMMatrixTranspose(W)); // Transpose for HLSL!;

	XMStoreFloat4x4(&rot4X4, XMMatrixTranspose(W)); // Transpose for HLSL!;
}

GameEntity::~GameEntity()
{
}

// getters

XMFLOAT4X4 GameEntity::GetWorldMatrix()
{
	return worldMatrix;
}

XMFLOAT4X4 GameEntity::GetTranslationMatrix4X4()
{
	return trans4X4;
}

XMFLOAT4X4 GameEntity::GetScaleMatrix4X4()
{
	return scale4X4;
}

XMFLOAT4X4 GameEntity::GetRotMatrix4X4()
{
	return rot4X4;
}

Mesh * GameEntity::GetMeshPtr()
{
	return meshPtr;
}

// calcuate entity world matrix

void GameEntity::CalculateWorldMatrix()
{
	XMMATRIX translation = XMLoadFloat4x4(&trans4X4);

	XMMATRIX rotation = XMLoadFloat4x4(&rot4X4);

	XMMATRIX scale = XMLoadFloat4x4(&scale4X4);

	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(scale * rotation * translation));
}

// setters

void GameEntity::SetTranslationMatrix4X4(XMMATRIX trans)
{
	XMStoreFloat4x4(&trans4X4, trans);
}

void GameEntity::SetScaleMatrix4X4(XMMATRIX scale)
{
	XMStoreFloat4x4(&scale4X4, scale);
}

void GameEntity::SetRotMatrix4X4(XMMATRIX rot)
{
	XMStoreFloat4x4(&rot4X4, rot);
}

// prepare entity material

void GameEntity::PrepareMaterial(XMFLOAT4X4 cameraView, XMFLOAT4X4 cameraProjection)
{
	materialPtr->GetVertexShader()->SetMatrix4x4("world", worldMatrix);
	materialPtr->GetVertexShader()->SetMatrix4x4("view", cameraView);
	materialPtr->GetVertexShader()->SetMatrix4x4("projection", cameraProjection);

	materialPtr->GetPixelShader()->SetSamplerState("Sampler", materialPtr->samplerStatePtr);
	materialPtr->GetPixelShader()->SetShaderResourceView("DiffuseTexture", materialPtr->srvPtr);
}
