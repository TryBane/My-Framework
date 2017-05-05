#include "Keyboard.h"

bool Keyboard::KeyIsPressed( unsigned char keycode ) const
{
	return keyBools[ keycode ];
}

Keyboard::Event Keyboard::ReadKey()
{
	if( !keyEventQueue.empty() )
	{
		Keyboard::Event e = keyEventQueue.front();
		keyEventQueue.pop();
		return e;
	}
	else
	{
		return Keyboard::Event();
	}
}

bool Keyboard::KeyIsEmpty() const
{
	return keyEventQueue.empty();
}

void Keyboard::EmptyKey()
{
	std::swap( keyEventQueue,std::queue<Event>() );
}

void Keyboard::Empty()
{
	EmptyKey();
}

void Keyboard::OnKeyPressed( unsigned char keycode )
{
	keyBools[ keycode ] = true;
	keyEventQueue.push( Keyboard::Event( Keyboard::Event::Type::Press,keycode) );
	
	keyEventQueue.empty();
}

void Keyboard::OnKeyReleased( unsigned char keycode )
{
	keyBools[ keycode ] = false;
	keyEventQueue.push( Keyboard::Event( Keyboard::Event::Type::Release,keycode) );

	keyEventQueue.empty();
}