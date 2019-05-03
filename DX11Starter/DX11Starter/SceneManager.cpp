#include "SceneManager.h"

void SceneManager::Init()
{
	// Subscribe to the events that the SceneManager system will respond to
	eventBus->Subscribe(this, &SceneManager::MovePlayerLeft);
	eventBus->Subscribe(this, &SceneManager::MovePlayerRight);
	eventBus->Subscribe(this, &SceneManager::PlayerJump);
	eventBus->Subscribe(this, &SceneManager::PlayerReverseGravity);
	eventBus->Subscribe(this, &SceneManager::QuickAddEntity);

	// Physics
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new  btCollisionDispatcher(collisionConfiguration);
	broadphase = new  btDbvtBroadphase();
	solver = new  btSequentialImpulseConstraintSolver;
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);

	dynamicsWorld->setGravity(btVector3(0, -3.0f, 0));

	// Player Stuff
	jumpCount = 0;
	doubleJumpControl = false;
	reverseGravity = false;
}

void SceneManager::Update(float deltaT, float totalT)
{
	deltaTime = deltaT;
	totalTime = totalT;

	sceneCamera->Update(deltaT);
	PhysicsStep();
	CheckCollisionWithFloor();
	CameraFollow();

	
}

void SceneManager::PhysicsStep()
{
	for (int i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);

		btTransform transform = body->getWorldTransform();
		transform.setOrigin(btVector3(sceneEntities[i].GetPosition().x, sceneEntities[i].GetPosition().y, sceneEntities[i].GetPosition().z));
		transform.setRotation(btQuaternion(sceneEntities[i].GetRotation().y, sceneEntities[i].GetRotation().z, sceneEntities[i].GetRotation().x));
		body->getMotionState()->setWorldTransform(transform);

		dynamicsWorld->stepSimulation(deltaTime*0.5f);

		body->getMotionState()->getWorldTransform(transform);

		sceneEntities[i].SetPosition(transform.getOrigin().getX(), transform.getOrigin().getY(), transform.getOrigin().getZ());
		sceneEntities[i].SetRotation(transform.getRotation().getX(), transform.getRotation().getY(), transform.getRotation().getZ());
		sceneEntities[i].SetWorldMatrix();
	}

	sceneEntities[0].GetRBody()->setLinearVelocity(btVector3(0.0f, sceneEntities[0].GetRBody()->getLinearVelocity().getY(), 0.0f));
}

void SceneManager::ExitPhysics()
{
	if (dynamicsWorld)
	{
		int i;
		for (i = dynamicsWorld->getNumConstraints() - 1; i >= 0; i--)
		{
			dynamicsWorld->removeConstraint(dynamicsWorld->getConstraint(i));
		}
		for (i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
		{
			btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
			btRigidBody* body = btRigidBody::upcast(obj);
			if (body && body->getMotionState())
			{
				delete body->getMotionState();
			}
			dynamicsWorld->removeCollisionObject(obj);
			delete obj;
		}
	}
	//delete collision shapes
	for (int j = 0; j < sceneEntities.size(); j++)
	{
		btCollisionShape* shape = sceneEntities[j].GetCollShape();
		delete shape;
	}

	delete dynamicsWorld;
	dynamicsWorld = 0;

	delete solver;
	solver = 0;

	delete broadphase;
	broadphase = 0;

	delete dispatcher;
	dispatcher = 0;

	delete collisionConfiguration;
	collisionConfiguration = 0;
}

void SceneManager::AddEntityToScene(GameEntity entity)
{
	sceneEntities.push_back(entity);

	// Add to bullet physics
	dynamicsWorld->addRigidBody(entity.GetRBody());
}

std::vector<GameEntity> * SceneManager::GetSceneEntities()
{
	return &sceneEntities;
}

void SceneManager::MovePlayerLeft(InputMoveLeft * inputEvent)
{
	sceneEntities[0].GetRBody()->activate();
	sceneEntities[0].GetRBody()->setLinearVelocity(btVector3(0.0f, sceneEntities[0].GetRBody()->getLinearVelocity().getY(), 0.0f));
	sceneEntities[0].GetRBody()->applyCentralImpulse(btVector3(-1.0f, 0.0f, 0.0f));
}

void SceneManager::MovePlayerRight(InputMoveRight * inputEvent)
{
	sceneEntities[0].GetRBody()->activate();
	sceneEntities[0].GetRBody()->setLinearVelocity(btVector3(0.0f, sceneEntities[0].GetRBody()->getLinearVelocity().getY(), 0.0f));
	sceneEntities[0].GetRBody()->applyCentralImpulse(btVector3(1.0f, 0.0f, 0.0f));
}

void SceneManager::PlayerJump(InputJump * inputEvent)
{
  	jumpCount++;

	if (jumpCount < 3)
	{
		sceneEntities[0].GetRBody()->activate();
		if (jumpCount == 1 && !doubleJumpControl)
		{
			sceneEntities[0].GetRBody()->setLinearVelocity(btVector3(sceneEntities[0].GetRBody()->getLinearVelocity().getX(), 0.0f, 0.0f));
			if (!reverseGravity)
			{
				sceneEntities[0].GetRBody()->applyCentralImpulse(btVector3(0.0f, 4.0f, 0.0f));
			}
			else
			{
				sceneEntities[0].GetRBody()->applyCentralImpulse(btVector3(0.0f, -4.0f, 0.0f));
			}
			doubleJumpControl = true;
		}
		if (jumpCount == 2 && doubleJumpControl)
		{
   			sceneEntities[0].GetRBody()->setLinearVelocity(btVector3(sceneEntities[0].GetRBody()->getLinearVelocity().getX(), 0.0f, 0.0f));
			if (!reverseGravity)
			{
				sceneEntities[0].GetRBody()->applyCentralImpulse(btVector3(0.0f, 4.0f, 0.0f));
			}
			else
			{
				sceneEntities[0].GetRBody()->applyCentralImpulse(btVector3(0.0f, -4.0f, 0.0f));
			}
			doubleJumpControl = false;
		}
	}
}

void SceneManager::PlayerReverseGravity(InputReverseGravity * inputEvent)
{
	sceneEntities[0].GetRBody()->activate();

	if (!reverseGravity)
	{
		sceneEntities[0].GetRBody()->setGravity(btVector3(0.0f, 3.0f, 0.0f));
		sceneEntities[0].GetRBody()->setLinearVelocity(btVector3(0.0f, 0.0f, 0.0f));
		reverseGravity = true;
	}
	else
	{
		sceneEntities[0].GetRBody()->setGravity(btVector3(0.0f, -3.0f, 0.0f));
		sceneEntities[0].GetRBody()->setLinearVelocity(btVector3(0.0f, 0.0f, 0.0f));
		reverseGravity = false;
	}
}

void SceneManager::QuickAddEntity(InputQuickAddEntity * inputEvent)
{
	GameEntity temp = GameEntity(sceneEntities[0].entityName, sceneEntities[0].GetMesh(), sceneEntities[0].GetMaterial(), sceneEntities[0].GetDeviceContext(), XMFLOAT3(0, 1, 0), sceneEntities[0].GetScale(), sceneEntities[0].GetRotation(), 1.0f);
	AddEntityToScene(temp);
}

void SceneManager::CameraFollow()
{
	sceneCamera->SetPosition(sceneEntities[0].GetPosition().x, sceneEntities[0].GetPosition().y, sceneCamera->GetPosition().z);
}

void SceneManager::CheckCollisionWithFloor()
{
	int numManifolds = dispatcher->getNumManifolds();
	for (int i = 0; i < numManifolds; i++)
	{
		btPersistentManifold* contactManifold = dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
		auto* obA = contactManifold->getBody0();
		auto* obB = contactManifold->getBody1();
		
		if (obA->getUserPointer() == "Player" && obB->getUserPointer() == "Floor")
		{
			int numContacts = contactManifold->getNumContacts();
			if (numContacts > 0 && !doubleJumpControl)
			{
				jumpCount = 0;
				doubleJumpControl = false;
			}
		}
	}
}
