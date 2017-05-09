#include "Game.h"

Game::Game( CoreWindow& Window )
	:
	Window( Window ),
	Graphics( Window )
{}

void Game::RunGame()
{
	Update();
	Graphics.Update();
	Graphics.Render();
}

void Game::Update()
{
	float x = 0, y = 0, z = 0;
	if( Window.keyboard.KeyIsPressed( VK_UP ) )
	{
		x = 0.2f;
	}

	if( Window.keyboard.KeyIsPressed( VK_DOWN ) )
	{
		x = -0.2f;
	}

	if( Window.keyboard.KeyIsPressed( VK_LEFT ) )
	{
		y = 0.2f;
	}
	
	if( Window.keyboard.KeyIsPressed( VK_RIGHT ) )
	{
		y = -0.2f;
	}
	
	Graphics.SetRotation( x, y, z );
}