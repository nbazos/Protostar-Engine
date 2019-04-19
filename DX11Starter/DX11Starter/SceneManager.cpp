#include "SceneManager.h"

void SceneManager::Init()
{
	// Subscribe to the events that the SceneManager system will respond to
	eventBus->Subscribe(this, &SceneManager::MovePlayerLeft);
	eventBus->Subscribe(this, &SceneManager::MovePlayerRight);

	// Physics
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new  btCollisionDispatcher(collisionConfiguration);
	broadphase = new  btDbvtBroadphase();
	solver = new  btSequentialImpulseConstraintSolver;
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);

	dynamicsWorld->setGravity(btVector3(0, -9.8f, 0));
}

void SceneManager::Update(float deltaT, float totalT)
{
	deltaTime = deltaT;
	totalTime = totalT;
	sceneCamera->Update(deltaT);
	PhysicsUpdate();
}

void SceneManager::PhysicsUpdate()
{
	for (int i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);

		btTransform transform = body->getWorldTransform();
		transform.setOrigin(btVector3(sceneEntities[i].GetPosition().x, sceneEntities[i].GetPosition().y, sceneEntities[i].GetPosition().z));
		body->setWorldTransform(transform);

		dynamicsWorld->stepSimulation(deltaTime);

		body->getMotionState()->getWorldTransform(transform);

		sceneEntities[i].SetPosition(transform.getOrigin().getX(), transform.getOrigin().getY(), transform.getOrigin().getZ());
	}
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
	// m_collisionShapes.clear();

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
	/*sceneEntities[0].MoveAbsolute(-2.0f * deltaTime, 0.0f, 0.0f);*/
	sceneEntities[0].GetRBody()->activate();
	sceneEntities[0].GetRBody()->applyCentralImpulse(btVector3(-25.0f * deltaTime, 0.0f, 0.0f));
}

void SceneManager::MovePlayerRight(InputMoveRight * inputEvent)
{
	/*sceneEntities[0].MoveAbsolute(2.0f * deltaTime, 0.0f, 0.0f);*/
	sceneEntities[0].GetRBody()->activate();
	sceneEntities[0].GetRBody()->applyCentralImpulse(btVector3(25.0f * deltaTime, 0.0f, 0.0f));
}