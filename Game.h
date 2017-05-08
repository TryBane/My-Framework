#pragma once
#include <windows.h>
#include "Graphics Engine\CoreGraphics.h"
#include "Graphics Engine\Window.h"

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

};