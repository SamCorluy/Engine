#pragma once
#include "Singleton.h"
#include "BaseCommand.h"

namespace dae
{
	enum class InputType
	{
		Press = 0,
		Release = 1,
		Hold = 2
	};
	struct Input
	{
		int input;
		bool isController;
		InputType type;
		bool operator< (const Input& comp) const
		{
			return input < comp.input || input == comp.input && type < comp.type;
		}

		bool operator> (const Input& comp) const
		{
			return input > comp.input || input == comp.input && type > comp.type;
		}

		bool operator== (const Input& comp) const
		{
			return input == comp.input && type == comp.type && isController == comp.isController;
		}
	};

	class InputManager final : public Singleton<InputManager>
	{
		class Impl;
		Impl* m_pImpl;
	public:
		InputManager();
		~InputManager();
		bool ProcessInput();
		void AddControllerInput(int input, InputType type, const  std::shared_ptr<BaseCommand>& command);
		void AddKeyboardInput(int input, InputType type, const  std::shared_ptr<BaseCommand>& command);
		void RemoveKeys();
		//bool checkButtons(ControllerButton button, XINPUT_STATE toCheck, XINPUT_STATE toCompare) const;
	};
}
