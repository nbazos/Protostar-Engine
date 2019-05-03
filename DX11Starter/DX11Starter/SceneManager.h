#pragma once

#include "System.h"
#include "GameEntity.h"
#include "Camera.h"

class SceneManager : public System {
public:
	SceneManager() {};
	SceneManager(EventBus * eventBusPtr, Camera * camera) : System(eventBusPtr), sceneCamera{ camera } {};
	~SceneManager() {};

	void Init();
	void Update(float deltaT, float totalT);

	void PhysicsStep();
	void ExitPhysics();

	void AddEntityToScene(GameEntity entity);
	//void RemoveEntityFromScene(char * entityName);
	std::vector<GameEntity>* GetSceneEntities();

private:
	std::vector<GameEntity> sceneEntities;
	Camera* sceneCamera;
	float deltaTime;
	float totalTime;

	// Player Control
	void MovePlayerLeft(InputMoveLeft * inputEvent);
	void MovePlayerRight(InputMoveRight * inputEvent);
	void PlayerJump(InputJump * inputEvent);
	void PlayerReverseGravity(InputReverseGravity * inputEvent);
	int jumpCount;
	bool doubleJumpControl;
	bool reverseGravity;

	// Utility
	void QuickAddEntity(InputQuickAddEntity * inputEvent);
	void CameraFollow();

	// Physics
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btBroadphaseInterface* broadphase;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;
	void CheckCollisionWithFloor();
};

