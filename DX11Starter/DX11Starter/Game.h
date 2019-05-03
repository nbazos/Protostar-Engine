#pragma once

#include "EventBus.h"
#include "Input.h"
#include "SceneManager.h"
#include "Renderer.h"
// #include "BulletPhysics.h"


class Game : public DXCore {
public:
	Game(HINSTANCE hInstance);
	~Game();

	// Overridden setup and game loop methods, which
	// will be called automatically
	void Init();
	void OnResize();
	void Update(float deltaTime, float totalTime);
	void Draw(float deltaTime, float totalTime);

	// Overridden mouse input helper methods
	void OnMouseDown (WPARAM buttonState, int x, int y);
	void OnMouseUp	 (WPARAM buttonState, int x, int y);
	void OnMouseMove (WPARAM buttonState, int x, int y);
	void OnMouseWheel(float wheelDelta,   int x, int y);
	void OnKeyDown(WPARAM keyCode, LPARAM keyDetails);
	void OnKeyUp(WPARAM keyCode, LPARAM keyDetails);

private:
	// Keeps track of the old mouse position.  Useful for 
	// determining how far the mouse moved in a single frame.
	POINT prevMousePos;

	Camera* camera;	

	// Engine systems
	EventBus eventBus;
	Input inputSystem;
	SceneManager sceneManager;
	Renderer renderSystem;
	// BulletPhysics physicsSystem;
};

