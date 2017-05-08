#include "Game.h"
#include "Graphics Engine\Window.h"

Game::Game( CoreWindow& Window )
	:
	Window( Window ),
	Graphics( Window )
{}

void Game::RunGame()
{
	Graphics.Update();
	Graphics.Render();
}