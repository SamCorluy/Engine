#pragma once
#include "Singleton.h"
#include "BaseCommand.h"

namespace dae
{
	enum class InputType
	{
		Press,
		Release,
		Hold
	};
	struct Input
	{
		int input;
		bool isController;
		InputType type;
		bool operator< (const Input& comp) const
		{
			return input < comp.input;
		}

		bool operator> (const Input& comp) const
		{
			return input > comp.input;
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
		void AddControllerInput(int input, InputType type, std::shared_ptr<BaseCommand>& command);
		void AddKeyboardInput(int input, InputType type, std::shared_ptr<BaseCommand>& command);
		//bool checkButtons(ControllerButton button, XINPUT_STATE toCheck, XINPUT_STATE toCompare) const;
	};
}
