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
};

