#pragma once

#include "System.h"
#include "GameEntity.h"
#include "Camera.h"

class SceneManager : public System {
public:
	SceneManager() {};
	SceneManager(EventBus * eventBusPtr, Camera * camera) : System(eventBusPtr), sceneCamera{ camera } {};
	~SceneManager() {};

	/*for (int i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			delete  body->getMotionState();
		}
		dynamicsWorld->removeCollisionObject(obj);
		delete  obj;
	}

	for (int j = 0; j < collisionShapes.size(); j++)
	{
		btCollisionShape* shape = collisionShapes[j];
		collisionShapes[j] = 0;
		delete shape;
	}

	delete dynamicsWorld;
	delete solver;
	delete broadphase;
	delete dispatcher;
	delete collisionConfiguration;*/

	void Init();
	void Update(float deltaT, float totalT);
	void PhysicsUpdate();

	void AddEntityToScene(GameEntity entity);
	//void RemoveEntityFromScene(char * entityName);
	std::vector<GameEntity>* GetSceneEntities();

private:
	std::vector<GameEntity> sceneEntities;
	Camera* sceneCamera;
	float deltaTime;
	float totalTime;

	void MovePlayerLeft(InputMoveLeft * inputEvent);
	void MovePlayerRight(InputMoveRight * inputEvent);

	// Physics
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btBroadphaseInterface* broadphase;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;
};

