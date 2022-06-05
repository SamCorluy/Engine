#include "MiniginPCH.h"
#include "InputManager.h"

#define WIN32_LEAN_AND_MEAN
#include "InputManager.h"
#include <XInput.h>
#include <map>
#pragma comment(lib, "XInput.lib")

class Engine::InputManager::Impl
{
	std::map <Input, std::vector<std::pair<std::shared_ptr<BaseCommand>, bool>>> m_Commands;
	std::map < SDL_Keycode,std::pair<bool, bool>> m_KeyQueue;
	std::map <WORD,std::pair<bool, bool>> m_ButtonQueue;
	XINPUT_KEYSTROKE m_CurrentState{};
	bool m_Reset = false;
public:
	Impl()
	{
		ZeroMemory(&m_CurrentState, sizeof(XINPUT_KEYSTROKE));
	}

	bool ProcessInput()
	{
		if (m_Reset)
		{
			std::vector<Input> in{};
			m_Reset = false;
			for (auto& command : m_Commands)
			{
				command.second.erase(std::remove_if(command.second.begin(), command.second.end(), [](std::pair<std::shared_ptr<BaseCommand>, bool>& object) { return object.second; }), command.second.end());
				if (command.second.empty())
					in.push_back(command.first);
			}
			for (auto input : in)
				m_Commands.erase(input);
		}
		for (auto key : m_KeyQueue)
			m_KeyQueue[key.first].second = true;
		SDL_Event e;
		std::vector<SDL_Keycode> toRemove;
		while (SDL_PollEvent(&e)) {
			switch (e.type)
			{
			case SDL_KEYDOWN:
				if (m_KeyQueue.find(e.key.keysym.sym) == m_KeyQueue.end())
					m_KeyQueue[e.key.keysym.sym] = std::pair<bool, bool>(true, false);
				break;
			case SDL_KEYUP:
				m_KeyQueue[e.key.keysym.sym] = std::pair<bool, bool>(false, (bool)e.key.repeat);
				toRemove.push_back(e.key.keysym.sym);
				break;
			}

			if (e.type == SDL_QUIT)
				return false;
		}
		ZeroMemory(&m_CurrentState, sizeof(XINPUT_KEYSTROKE));
		for (auto button : m_ButtonQueue)
			m_ButtonQueue[button.first].second = true;
		std::vector<WORD> xToRemove;
		while (XInputGetKeystroke(0, 0, &m_CurrentState) == ERROR_SUCCESS) {
			switch (m_CurrentState.Flags)
			{
			case XINPUT_KEYSTROKE_KEYDOWN:
				if (m_ButtonQueue.find(m_CurrentState.VirtualKey) == m_ButtonQueue.end())
					m_ButtonQueue[m_CurrentState.VirtualKey] = std::pair<bool, bool>(true, false);
				break;
			case XINPUT_KEYSTROKE_KEYUP:
				m_ButtonQueue[m_CurrentState.VirtualKey] = std::pair<bool, bool>(false, m_CurrentState.Flags == XINPUT_KEYSTROKE_REPEAT);
				xToRemove.push_back(m_CurrentState.VirtualKey);
				break;
			}
		}
		for (auto key: m_KeyQueue)
		{
			for (auto command : m_Commands)
			{
				auto input = command.first;
				auto commands = command.second;

				if (input.isController)
					continue;
				if (input.input == key.first)
				{
					switch (input.type)
					{
					case InputType::Hold:
						if (key.second.first && key.second.second)
							for (auto com : commands)
								com.first->Execute();
						break;
					case InputType::Press:
						if (key.second.first && !key.second.second)
							for (auto com : commands)
								com.first->Execute();
						break;
					case InputType::Release:
						if (!key.second.first)
							for (auto com : commands)
								com.first->Execute();
						break;
					}
				}
			}
		}
		for (auto idx : toRemove)
			m_KeyQueue.erase(idx);

		for (auto button : m_ButtonQueue)
		{
			for (auto command : m_Commands)
			{
				auto input = command.first;
				auto commands = command.second;

				if (!input.isController)
					continue;
				if (input.input == button.first)
				{
					switch (input.type)
					{
					case InputType::Hold:
						if (button.second.first)
							for (auto com : commands)
								com.first->Execute();
						break;
					case InputType::Press:
						if (button.second.first && !button.second.second)
							for (auto com : commands)
								com.first->Execute();
						break;
					case InputType::Release:
						if (!button.second.first)
							for (auto com : commands)
								com.first->Execute();
						break;
					}
				}
			}
		}
		for (auto idx : xToRemove)
			m_ButtonQueue.erase(idx);
		return true;
	}

	void AddControllerInput(int input, InputType type, const std::shared_ptr<BaseCommand>& command)
	{
		Input i;
		i.input = input;
		i.type = type;
		i.isController = true;
		m_Commands[{input, true, type}].push_back({ command, false });
	}

	void AddKeyboardInput(int input, InputType type, const std::shared_ptr<BaseCommand>& command)
	{
		Input i;
		i.input = input;
		i.type = type;
		i.isController = false;
		m_Commands[{input, false, type}].push_back({ command, false });
	}

	void RemoveKeys()
	{
		m_Reset = true;
		for (auto& command : m_Commands)
		{
			for(auto& com : command.second)
				com.second = true;
		}
	}
};

void Engine::InputManager::AddControllerInput(int input, InputType type, const std::shared_ptr<BaseCommand>& command)
{
	m_pImpl->AddControllerInput(input, type, command);
}

void Engine::InputManager::AddKeyboardInput(int input, InputType type, const std::shared_ptr<BaseCommand>& command)
{
	m_pImpl->AddKeyboardInput(input, type, command);
}

Engine::InputManager::InputManager()
{
	m_pImpl = new Impl();
}

Engine::InputManager::~InputManager()
{
	delete m_pImpl;
}

void Engine::InputManager::RemoveKeys()
{
	m_pImpl->RemoveKeys();
}

bool Engine::InputManager::ProcessInput()
{
	return m_pImpl->ProcessInput();
}

