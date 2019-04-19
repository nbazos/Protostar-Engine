#pragma once

#include <DirectXMath.h>
#include "Mesh.h"
#include "Material.h"
#include "btBulletDynamicsCommon.h"
// For the DirectX Math library
using namespace DirectX;

class GameEntity {
public:
	GameEntity(const char * name, Mesh* mesh, Material* material, ID3D11DeviceContext* context, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation, float isStatic);
	~GameEntity();

	void SetWorldMatrix();
	void SetPosition(float posX, float posY, float posZ);
	void SetScale(float scaleX, float scaleY, float scaleZ);
	void SetRotation(float rotX, float rotY, float rotZ);

	DirectX::XMFLOAT3 GetScale() { return scale; };
	DirectX::XMFLOAT3 GetRotation() { return rotation; };
	DirectX::XMFLOAT3 GetPosition() { return position; };
	DirectX::XMFLOAT4X4 GetWorldMatrix() { return worldMatrix; };

	btRigidBody * GetRBody();
	btCollisionShape * GetCollShape();

	void MoveAbsolute(float translationX, float translationY, float translationZ);
	void Draw(XMFLOAT4X4 viewMat, XMFLOAT4X4 projectionMat);

	const char * entityName;
private:
	XMFLOAT4X4 worldMatrix;
	XMFLOAT3 position;
	XMFLOAT3 scale;
	XMFLOAT3 rotation;

	Mesh* entityMesh;
	Material* entityMaterial;
	ID3D11DeviceContext* deviceContext;

	btCollisionShape* collShape;
	btRigidBody* rBody;

	void PrepareMaterials(XMFLOAT4X4 viewMat, XMFLOAT4X4 projectionMat);
};