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
	std::vector<GameEntity>* GetSceneEntities();

private: // Members
	std::vector<GameEntity> sceneEntities;
	Camera* sceneCamera;
	float deltaTime;
	float totalTime;
	int jumpCount;
	bool doubleJumpControl;	
	
	// Physics
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btBroadphaseInterface* broadphase;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;

private: // Methods
	void RemoveEntityFromScene(int entityIndex);

	// Player Control
	void MovePlayerLeft(InputMoveLeft * inputEvent);
	void MovePlayerRight(InputMoveRight * inputEvent);
	void PlayerJump(InputJump * inputEvent);
	
	void CameraFollow();
	void QuickAddEntity(InputQuickAddEntity * inputEvent);
	void CheckCollisionWithFloor();
	void CleanUpEntities();
};

