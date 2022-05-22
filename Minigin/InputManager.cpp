#include "MiniginPCH.h"
#include "InputManager.h"

#define WIN32_LEAN_AND_MEAN
#include "InputManager.h"
#include <XInput.h>
#include <map>
#pragma comment(lib, "XInput.lib")

class dae::InputManager::Impl
{
	std::map <Input, std::vector<std::shared_ptr<BaseCommand>>> m_Commands;
	XINPUT_KEYSTROKE m_CurrentState{};
public:
	Impl()
	{
		ZeroMemory(&m_CurrentState, sizeof(XINPUT_KEYSTROKE));
	}

	bool ProcessInput()
	{
		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			for (auto command : m_Commands)
			{
				auto input = command.first;
				auto commands = command.second;

				if (input.isController)
					continue;
				if (input.input == e.key.keysym.sym)
				{
					switch (input.type)
					{
					case InputType::Hold:
					case InputType::Press:
						if (e.type == SDL_KEYDOWN)
							for (auto com : commands)
								com->Execute();
						break;
					case InputType::Release:
						if (e.type == SDL_KEYUP)
							for (auto com : commands)
								com->Execute();
						break;
					}
				}
			}

			if (e.type == SDL_QUIT)
				return false;
		}
		ZeroMemory(&m_CurrentState, sizeof(XINPUT_KEYSTROKE));

		while (XInputGetKeystroke(0, 0, &m_CurrentState) == ERROR_SUCCESS)
		{
			for (auto command : m_Commands)
			{
				auto input = command.first;
				auto commands = command.second;

				if (!input.isController)
					continue;
				if (input.input == m_CurrentState.VirtualKey)
				{
					switch (input.type)
					{
					case InputType::Hold:
						if (m_CurrentState.Flags == XINPUT_KEYSTROKE_KEYDOWN)
							for (auto com : commands)
								com->Execute();
						break;
					case InputType::Press:
						if (m_CurrentState.Flags == XINPUT_KEYSTROKE_KEYDOWN && m_CurrentState.Flags != XINPUT_KEYSTROKE_REPEAT)
							for (auto com : commands)
								com->Execute();
						break;
					case InputType::Release:
						if (m_CurrentState.Flags == XINPUT_KEYSTROKE_KEYUP)
							for (auto com : commands)
								com->Execute();
						break;
					}
				}
			}
		}
		return true;
	}

	void AddControllerInput(int input, InputType type, std::shared_ptr<BaseCommand>& command)
	{
		Input i;
		i.input = input;
		i.type = type;
		i.isController = true;
		m_Commands[i].push_back(command);
	}

	void AddKeyboardInput(int input, InputType type, std::shared_ptr<BaseCommand>& command)
	{
		Input i;
		i.input = input;
		i.type = type;
		i.isController = false;
		m_Commands[i].push_back(command);
	}
};

void dae::InputManager::AddControllerInput(int input, InputType type, std::shared_ptr<BaseCommand>& command)
{
	m_pImpl->AddControllerInput(input, type, command);
}

void dae::InputManager::AddKeyboardInput(int input, InputType type, std::shared_ptr<BaseCommand>& command)
{
	m_pImpl->AddKeyboardInput(input, type, command);
}

dae::InputManager::InputManager()
{
	m_pImpl = new Impl();
}

dae::InputManager::~InputManager()
{
	delete m_pImpl;
}

bool dae::InputManager::ProcessInput()
{
	return m_pImpl->ProcessInput();
}

