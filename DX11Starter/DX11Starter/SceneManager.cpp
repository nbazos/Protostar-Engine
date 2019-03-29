#include "SceneManager.h"

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::Init()
{
	// Subscribe to the events that the SceneManager system will respond to
	eventBus->Subscribe(this, &SceneManager::MovePlayerLeft);
	eventBus->Subscribe(this, &SceneManager::MovePlayerRight);
}

void SceneManager::Update(float deltaT, float totalT)
{
	deltaTime = deltaT;
	totalTime = totalT;
}

void SceneManager::AddEntityToScene(GameEntity entity)
{
	sceneEntities.push_back(entity);
}

std::vector<GameEntity> SceneManager::GetSceneEntities()
{
	return sceneEntities;
}

void SceneManager::MovePlayerLeft(InputMoveLeft * inputEvent)
{
	sceneEntities[0].MoveAbsolute(-2.0f * deltaTime, 0.0f, 0.0f);
}

void SceneManager::MovePlayerRight(InputMoveRight * inputEvent)
{
	sceneEntities[0].MoveAbsolute(2.0f * deltaTime, 0.0f, 0.0f);
}


