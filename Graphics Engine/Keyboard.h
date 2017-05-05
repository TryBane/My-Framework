#pragma once
#include <queue>
#include <bitset>

class Keyboard
{
	friend class Window;
public:
	class Event
	{
	public:
		enum class Type
		{
			Default,
			Press,
			Release
		};
	private:
		Type inputType;
		unsigned char keyCode;
	public:
		Event() = default;
		Event( Type inputType,unsigned char keyCode )
			:
		inputType(inputType),
		keyCode(keyCode)
		{}
		bool IsPress() const
		{
			return ( inputType == Type::Press );
		}
		bool IsRelease() const
		{
			return ( inputType == Type::Release );
		}
		bool IsNotDefault() const
		{
			return ( inputType != Type::Default );
		}
		unsigned char GetKeyCode( ) const
		{
			return keyCode;
		}
	};
public:
	Keyboard() = default;
	bool KeyIsPressed( unsigned char keycode ) const;
	Event ReadKey();
	bool KeyIsEmpty() const;
	void EmptyKey();
	void Empty();
	void OnKeyPressed( unsigned char keycode );
	void OnKeyReleased( unsigned char keycode );
private:
	static constexpr unsigned int nKeys = 256u;
	static constexpr unsigned int bufferSize = 4u;
	std::bitset<nKeys> keyBools;
	std::queue<Event> keyEventQueue;
};