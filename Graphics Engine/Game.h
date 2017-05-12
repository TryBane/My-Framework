#pragma once
#include <windows.h>
#include "Window.h"
#include "CoreGraphics.h"
#include "btBulletDynamicsCommon.h"
#include <memory>

class Game
{
public:
	Game( class CoreWindow& Window );
	Game() = delete;
	void RunSimulation();
	void RunGame();
	void Update();
	void CheckState();
private:
	CoreWindow& Window;
	CoreGraphics Graphics;
	btDispatcher* dispatcher;
	btCollisionConfiguration* collisionConfig = new btDefaultCollisionConfiguration();
	btBroadphaseInterface* broadphase;
	btConstraintSolver* solver;
	btDynamicsWorld* world = new btDiscreteDynamicsWorld( dispatcher,broadphase,solver,collisionConfig );
};