#include "GameEntity.h"

GameEntity::GameEntity(Mesh* mesh, Material* material, ID3D11DeviceContext* context)
{
	entityMesh = mesh;
	entityMaterial = material;
	deviceContext = context;

	XMMATRIX W = XMMatrixIdentity();
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(W));
	position = XMFLOAT3(0, 0, 0);
	scale = XMFLOAT3(1, 1, 1);
	rotation = XMFLOAT3(0, 0, 0);
}

GameEntity::~GameEntity()
{
}

void GameEntity::SetWorldMatrix()
{
	XMMATRIX pos = XMMatrixTranslation(position.x, position.y, position.z);
	XMMATRIX scal = XMMatrixScaling(scale.x, scale.y, scale.z);
	XMVECTOR rotQuat = XMQuaternionRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
	XMMATRIX rotXYZ = XMMatrixRotationQuaternion(rotQuat);
	XMMATRIX worldMat = scal * rotXYZ * pos;
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(worldMat));
}

void GameEntity::SetPosition(float posX, float posY, float posZ)
{
	position.x = posX;
	position.y = posY;
	position.z = posZ;
}

void GameEntity::SetScale(float scalar)
{
	scale.x = scalar;
	scale.y = scalar;
	scale.z = scalar;
}

void GameEntity::SetRotation(float pitch, float yaw, float roll)
{
	rotation.x = pitch * XM_PI / 180;
	rotation.y = yaw * XM_PI / 180;
	rotation.z = roll * XM_PI / 180;
}

void GameEntity::Draw(XMFLOAT4X4 viewMat, XMFLOAT4X4 projectionMat)
{
	PrepareMaterials(viewMat, projectionMat);
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	ID3D11Buffer* vertexBuffer = entityMesh->GetVertexBuffer();
	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(entityMesh->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);
	deviceContext->DrawIndexed(
		entityMesh->GetIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
		0,     // Offset to the first index we want to use
		0);    // Offset to add to each index when looking up vertices
}

void GameEntity::PrepareMaterials(XMFLOAT4X4 viewMat, XMFLOAT4X4 projectionMat)
{
	// Send data to shader variables
	entityMaterial->GetVertexShader()->SetMatrix4x4("world", worldMatrix);
	entityMaterial->GetVertexShader()->SetMatrix4x4("view", viewMat);
	entityMaterial->GetVertexShader()->SetMatrix4x4("projection", projectionMat);

	entityMaterial->GetPixelShader()->SetShaderResourceView("diffuseTexture", entityMaterial->GetSRV());
	entityMaterial->GetPixelShader()->SetSamplerState("basicSampler", entityMaterial->GetSamplerState());

	// Send data to GPU
	entityMaterial->GetVertexShader()->CopyAllBufferData();
	entityMaterial->GetPixelShader()->CopyAllBufferData();

	// Set the vertex and pixel shaders to use for the next Draw() command
	entityMaterial->GetVertexShader()->SetShader();
	entityMaterial->GetPixelShader()->SetShader();
}


