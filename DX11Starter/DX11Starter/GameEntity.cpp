#include "GameEntity.h"

GameEntity::GameEntity(char * name, Mesh* mesh, Material* material, ID3D11DeviceContext* context, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation, float isStatic)
{
	entityName = name;
	entityMesh = mesh;
	entityMaterial = material;
	deviceContext = context;
	this->isStatic = isStatic;

	XMMATRIX W = XMMatrixIdentity();
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(W));
	this->position = position;
	this->scale = scale;
	this->rotation = rotation;

	// Physics

	if (entityName == "Player")
	{
		//this->collShape = new btCapsuleShape(scale.x/2.0f, (float)scale.y);
		this->collShape = new btSphereShape(scale.x / 2.0f);
	}
	else
	{
		this->collShape = new btBoxShape(btVector3(btScalar(scale.x / 2.0f), btScalar(scale.y / 2.0f), btScalar(scale.z / 2.0f)));
	}

	btTransform groundTransform;
	groundTransform.setIdentity();
	groundTransform.setOrigin(btVector3(position.x, position.y, position.z));

	btScalar mass(isStatic);

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (mass != 0.f);

	btVector3 localInertia(0, 0, 0);
	if (isDynamic)
		collShape->calculateLocalInertia(mass, localInertia);

	//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, collShape, localInertia);
	this->rBody = new btRigidBody(rbInfo);

	// Constrainments
	rBody->setLinearFactor(btVector3(1,1,0));
	rBody->setAngularFactor(btVector3(0, 1, 1));

	if (entityName == "Player")
	{
		rBody->setAnisotropicFriction(btVector3(1.0f, 0.0f, 0.0f));
		rBody->setRestitution(0.8f);
		//rBody->hasContactResponse();
	}
	if (entityName == "Floor" || entityName == "Platform1" || entityName == "Platform2")
	{
		rBody->setAnisotropicFriction(btVector3(1.0f, 0.0f, 0.0f));
		//rBody->setRollingFriction(0.2f);
		rBody->setRestitution(0.8f);
		//rBody->hasContactResponse();
	}
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

void GameEntity::SetScale(float scaleX, float scaleY, float scaleZ)
{
	scale.x = scaleX;
	scale.y = scaleY;
	scale.z = scaleZ;
}

void GameEntity::SetRotation(float pitch, float yaw, float roll)
{
	rotation.x = pitch * XM_PI / 180;
	rotation.y = yaw * XM_PI / 180;
	rotation.z = roll * XM_PI / 180;
}

btRigidBody * GameEntity::GetRBody()
{
	return rBody;
}

btCollisionShape * GameEntity::GetCollShape()
{
	return collShape;
}

Mesh * GameEntity::GetMesh()
{
	return entityMesh;
}

Material * GameEntity::GetMaterial()
{
	return entityMaterial;
}

ID3D11DeviceContext * GameEntity::GetDeviceContext()
{
	return deviceContext;
}

void GameEntity::MoveAbsolute(float translationX, float translationY, float translationZ)
{
	// publish an event type to go to bullet physics and set position there to solve input issue?

	position.x += translationX;
	position.y += translationY;
	position.z += translationZ;
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
	SetWorldMatrix(); // Recalculate world matrix 

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


