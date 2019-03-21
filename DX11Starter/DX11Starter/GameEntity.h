#pragma once

#include <DirectXMath.h>
#include "Mesh.h"
#include "Material.h"
#include "Lights.h"

using namespace DirectX;

class GameEntity
{
public:
	GameEntity(Mesh* meshPtr, Material* materialPtr);
	GameEntity();
	~GameEntity();

	// Member variables
	XMFLOAT4X4 worldMatrix;

	XMFLOAT4X4 trans4X4;
	XMFLOAT4X4 scale4X4;
	XMFLOAT4X4 rot4X4; // Quaternion Matrix

	Mesh* meshPtr;
	Material* materialPtr;

	// getters
	XMFLOAT4X4 GetWorldMatrix();
	XMFLOAT4X4 GetTranslationMatrix4X4();
	XMFLOAT4X4 GetScaleMatrix4X4();
	XMFLOAT4X4 GetRotMatrix4X4();
	Mesh* GetMeshPtr();

	// calculate world matrix for this entity
	void CalculateWorldMatrix();

	// setters
	void SetTranslationMatrix4X4(XMMATRIX position);
	void SetScaleMatrix4X4(XMMATRIX scale);
	void SetRotMatrix4X4(XMMATRIX rot);

	// prepare entity material
	void PrepareMaterial(XMFLOAT4X4 cameraView, XMFLOAT4X4 cameraProjection);
};

