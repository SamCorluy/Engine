#include "GameManagerComponent.h"
#include "Renderer.h"
#include "InputManager.h"
#include "CharacterMoveCommand.h"
#include "ThrowSaltCommand.h"
#include <queue>
#include <iostream>
#include "ElapsedTime.h"
#include "HealthComponent.h"
#include "HealthObserver.h"
#include "SaltDisplayComponent.h"
#include "SaltObserver.h"
#include "ResourceManager.h"

GameManagerComponent::GameManagerComponent(const std::shared_ptr<dae::GameObject> owner, const std::weak_ptr<dae::Scene>& scene)
	: BaseComponent(owner)
	, m_pScene{scene}
	, m_HotDogSpawnCoolDown{8.f}
	, m_CurrentHotDogSpawnCoolDown{ m_HotDogSpawnCoolDown }
	, m_PickleSpawnCoolDown{ 15.f }
	, m_CurrentPickleSpawnCoolDown{ m_PickleSpawnCoolDown }
	, m_EggSpawnCoolDown{ 15.f }
	, m_currentEggSpawnCoolDown{ m_EggSpawnCoolDown }
	, m_pPlayers{ std::shared_ptr<PeterPepperComponent>(nullptr), std::shared_ptr<PeterPepperComponent>(nullptr) }
{
	m_LevelFolders.push_back("Level1");
	m_LevelFolders.push_back("Level2");
	m_LevelFolders.push_back("Level3");
	InitLevel(0);
	//InitCoop();
	InitSinglePlayer();
}

void GameManagerComponent::Update()
{
	if (!m_pPlayers[0].expired() && m_pPlayers[0].lock()->IsDead())
	{
		if (m_pPlayers[0].lock()->DeathAnimationFinished())
			Reset();
		return;
	}
	bool completed = true;
	for (auto burger : m_pBurgers)
		if (!burger.lock()->IsComplete())
			completed = false;
	if (completed)
		NextLevel();
	m_pHotDogs.erase(std::remove_if(m_pHotDogs.begin(), m_pHotDogs.end(), [](std::weak_ptr<EnemyComponent>& enemy) { return enemy.expired(); }), m_pHotDogs.end());
	m_pEggs.erase(std::remove_if(m_pEggs.begin(), m_pEggs.end(), [](std::weak_ptr<EnemyComponent>& enemy) { return enemy.expired(); }), m_pEggs.end());
	m_pPickles.erase(std::remove_if(m_pPickles.begin(), m_pPickles.end(), [](std::weak_ptr<EnemyComponent>& enemy) { return enemy.expired(); }), m_pPickles.end());
	if (m_pHotDogs.size() < 5)
	{
		if (m_CurrentHotDogSpawnCoolDown > 0.f)
			m_CurrentHotDogSpawnCoolDown -= ElapsedTime::GetInstance().GetElapsedTime();
		if (m_CurrentHotDogSpawnCoolDown <= 0.f)
			SpawnHotDog();
	}
	if (m_pEggs.size() < 1)
	{
		if (m_currentEggSpawnCoolDown > 0.f)
			m_currentEggSpawnCoolDown -= ElapsedTime::GetInstance().GetElapsedTime();
		if (m_currentEggSpawnCoolDown <= 0.f)
			SpawnEgg();
	}
	if (m_pPickles.size() < 3)
	{
		if (m_CurrentPickleSpawnCoolDown > 0.f)
			m_CurrentPickleSpawnCoolDown -= ElapsedTime::GetInstance().GetElapsedTime();
		if (m_CurrentPickleSpawnCoolDown <= 0.f)
			SpawnPickle();
	}
	CheckBurgerOverlap();
	HandleFallingBurgers();
	HandleSalt();
	CheckPlayerOverlap();
	auto grid = m_pLevel.lock()->GetGrid();
	//m_TempPath = FindPath(grid[{0, 0}], m_pPlayer.lock()->getNode());
	DeterminePathEnemies(m_pEggs);
	DeterminePathEnemies(m_pHotDogs);
	DeterminePathEnemies(m_pPickles);
	//int kill = rand() % 101;
	//if (!m_pPlayer.expired() && kill == 5)
	//	m_pPlayer.lock()->Die();
}

void GameManagerComponent::StaticUpdate()
{
}

void GameManagerComponent::Render(const dae::Transform&) const
{
	//auto window = dae::Renderer::GetInstance().GetWindow();
	//int windowWidth;
	//int windowHeight;
	//SDL_GetWindowSize(window, &windowWidth, &windowHeight);
	//auto node = m_TempPath[0].lock();
	//auto trans = node->GetOwner().lock()->GetTransform().GetPosition();
	//int x1{ node->GetNodePos().first + node->GetNodeSize().first / 2 + (int)trans.x};
	//int y1{ windowHeight - (node->GetNodePos().second + node->GetNodeSize().second / 2 + (int)trans.y) };
	//for (size_t i = 1; i < m_TempPath.size(); ++i)
	//{
	//	SDL_SetRenderDrawColor(dae::Renderer::GetInstance().GetSDLRenderer(), 255, 255, 255, 1);
	//	node = m_TempPath[i].lock();
	//	int x2{ node->GetNodePos().first + node->GetNodeSize().first / 2 + (int)trans.x };
	//	int y2{ windowHeight - (node->GetNodePos().second + node->GetNodeSize().second / 2 + (int)trans.y) };
	//	SDL_RenderDrawLine(dae::Renderer::GetInstance().GetSDLRenderer(), x1, y1, x2, y2);
	//	x1 = x2;
	//	y1 = y2;
	//}
}

void GameManagerComponent::DeterminePathEnemies(std::vector<std::weak_ptr<EnemyComponent>>& enemies)
{
	for (auto enemy : enemies)
	{
		if (m_pPlayers[0].expired())
			continue;
		if (!enemy.lock()->ReachedChoicePoint())
			continue;
		m_TempPath = FindPath(enemy.lock()->getNode(), m_pPlayers[0].lock()->getNode(), enemy.lock()->getPrevNode());
		//std::cout << hotDog.lock()->getNode().lock()->GetNodePos().first << " " << hotDog.lock()->getPrevNode().lock()->GetNodePos().first << "\n";
		if (m_TempPath.size() >= 2)
			enemy.lock()->Move(m_TempPath[1]);
		else
		{
			for (auto node : m_pPlayers[0].lock()->getNode().lock()->GetConnections())
			{
				if (node.lock() != enemy.lock()->getPrevNode().lock())
				{
					enemy.lock()->Move(node);
					return;
				}
			}
		}
	}
}

void GameManagerComponent::InitSinglePlayer()
{
	InitPlayer(0, 1);
	SpawnHotDog();
	SpawnEgg();
	SpawnPickle();

	InitBurgers();
}

void GameManagerComponent::InitCoop()
{
	InitPlayer(0, 2);
	InitPlayer(1, 2);
	SpawnHotDog();
	SpawnEgg();
	SpawnPickle();

	InitBurgers();
}

void GameManagerComponent::CheckBurgerOverlap()
{
	for (size_t i = 0; i < m_pPlayers.size(); ++i)
	{
		if (m_pPlayers[i].expired())
			return;
		for (auto burger : m_pBurgers)
		{
			for (auto ingredient : burger.lock()->getIngredients())
			{
				bool wasDropped = ingredient.lock()->hasDropped();
				if (ingredient.lock()->CheckOverlap(m_pPlayers[i], i) && !wasDropped)
				{
					auto pos = ingredient.lock()->GetOwner().lock()->GetTransform().GetPosition();
					auto size = ingredient.lock()->GetRectSize();
					pos.x -= size.first / 2;
					int extraDrops{ 0 };
					Rect ingredientRect{ static_cast<int>(pos.x), static_cast<int>(pos.y), size.first, size.second };
					for (auto enemy : m_pHotDogs)
					{
						if (ingredient.lock()->GetNode().lock() != enemy.lock()->getNode().lock())
							continue;
						pos = enemy.lock()->GetOwner().lock()->GetTransform().GetPosition();
						size = enemy.lock()->GetRectSize();
						pos.x -= size.first / 2;
						Rect enemyRect{ static_cast<int>(pos.x), static_cast<int>(pos.y), size.first, size.second };
						if (CheckRectOverlap(ingredientRect, enemyRect))
						{
							enemy.lock()->Kill(m_pPlayers[i]);
							++extraDrops;
						}
					}
					for (auto enemy : m_pEggs)
					{
						if (ingredient.lock()->GetNode().lock() != enemy.lock()->getNode().lock())
							continue;
						pos = enemy.lock()->GetOwner().lock()->GetTransform().GetPosition();
						size = enemy.lock()->GetRectSize();
						pos.x -= size.first / 2;
						Rect enemyRect{ static_cast<int>(pos.x), static_cast<int>(pos.y), size.first, size.second };
						if (CheckRectOverlap(ingredientRect, enemyRect))
						{
							enemy.lock()->Kill(m_pPlayers[i]);
							++extraDrops;
						}
					}
					for (auto enemy : m_pPickles)
					{
						if (ingredient.lock()->GetNode().lock() != enemy.lock()->getNode().lock())
							continue;
						pos = enemy.lock()->GetOwner().lock()->GetTransform().GetPosition();
						size = enemy.lock()->GetRectSize();
						pos.x -= size.first / 2;
						Rect enemyRect{ static_cast<int>(pos.x), static_cast<int>(pos.y), size.first, size.second };
						if (CheckRectOverlap(ingredientRect, enemyRect))
						{
							enemy.lock()->Kill(m_pPlayers[i]);
							++extraDrops;
						}
					}
					//int amount = rand() % 3;
					ingredient.lock()->InitiateDrop(m_pPlayers[i], extraDrops);
					//std::cout << amount << "\n";
				}
			}
		}
	}
}

void GameManagerComponent::CheckPlayerOverlap()
{
	if (!m_pPlayers[0].expired() && !m_pPlayers[0].lock()->IsDead())
	{
		auto pos = m_pPlayers[0].lock()->GetOwner().lock()->GetTransform().GetPosition();
		auto size = m_pPlayers[0].lock()->GetRectSize();
		pos.x -= size.first / 2;
		Rect PlayerRect{ static_cast<int>(pos.x), static_cast<int>(pos.y), size.first, size.second };
		for (auto hotdog : m_pHotDogs)
		{
			if (m_pPlayers[0].lock()->getNode().lock() != hotdog.lock()->getNode().lock() || hotdog.lock()->IsStunned())
				continue;
			pos = hotdog.lock()->GetOwner().lock()->GetTransform().GetPosition();
			size = hotdog.lock()->GetRectSize();
			pos.x -= size.first / 2;
			Rect enemyRect{ static_cast<int>(pos.x), static_cast<int>(pos.y), size.first, size.second };
			if (CheckRectOverlap(PlayerRect, enemyRect))
			{
				m_pPlayers[0].lock()->Die();
				return;
			}
		}
		for (auto egg : m_pEggs)
		{
			if (m_pPlayers[0].lock()->getNode().lock() != egg.lock()->getNode().lock() || egg.lock()->IsStunned())
				continue;
			pos = egg.lock()->GetOwner().lock()->GetTransform().GetPosition();
			size = egg.lock()->GetRectSize();
			pos.x -= size.first / 2;
			Rect enemyRect{ static_cast<int>(pos.x), static_cast<int>(pos.y), size.first, size.second };
			if (CheckRectOverlap(PlayerRect, enemyRect))
			{
				m_pPlayers[0].lock()->Die();
				return;
			}
		}
		for (auto pickle : m_pPickles)
		{
			if (m_pPlayers[0].lock()->getNode().lock() != pickle.lock()->getNode().lock() || pickle.lock()->IsStunned())
				continue;
			pos = pickle.lock()->GetOwner().lock()->GetTransform().GetPosition();
			size = pickle.lock()->GetRectSize();
			pos.x -= size.first / 2;
			Rect enemyRect{ static_cast<int>(pos.x), static_cast<int>(pos.y), size.first, size.second };
			if (CheckRectOverlap(PlayerRect, enemyRect))
			{
				m_pPlayers[0].lock()->Die();
				return;
			}
		}
	}
}

const bool GameManagerComponent::CheckRectOverlap(Rect rectOne, Rect rectTwo)
{
	return (rectOne.x < rectTwo.x + rectTwo.width && rectOne.x + rectOne.width > rectTwo.x &&
		rectOne.y + rectOne.height > rectTwo.y && rectOne.y < rectTwo.y + rectTwo.height);
}

void GameManagerComponent::SpawnHotDog()
{
	m_CurrentHotDogSpawnCoolDown = m_HotDogSpawnCoolDown;
	auto grid = m_pLevel.lock()->GetGrid();
	auto startNode = grid[m_pLevel.lock()->GetHotDogSpawn()];
	AnimDurationInit animInit(0.25f, 0.25f, 0.25f, 0.5f, 0.3f);
	auto obj = std::make_shared<dae::GameObject>();
	obj->AddComponent<EnemyComponent>(std::make_shared<EnemyComponent>(obj, 3, startNode, m_pLevel.lock()->GetFloorOffset(), "Textures/HotDog", animInit, 100));
	//obj->GetComponent<dae::Subject>().lock()->AddObserver(m_pPointsObserver);
	m_pScene.lock()->Add(obj, 1);
	m_pHotDogs.push_back(obj->GetComponent<EnemyComponent>());
}

void GameManagerComponent::SpawnEgg()
{
	m_currentEggSpawnCoolDown = m_EggSpawnCoolDown;
	auto grid = m_pLevel.lock()->GetGrid();
	auto startNode = grid[m_pLevel.lock()->GetEggSpawn()];
	AnimDurationInit animInit(0.25f, 0.25f, 0.25f, 0.5f, 0.3f);
	auto obj = std::make_shared<dae::GameObject>();
	obj->AddComponent<EnemyComponent>(std::make_shared<EnemyComponent>(obj, 3, startNode, m_pLevel.lock()->GetFloorOffset(), "Textures/Egg", animInit, 300));
	//obj->GetComponent<dae::Subject>().lock()->AddObserver(m_pPointsObserver);
	m_pScene.lock()->Add(obj, 1);
	m_pEggs.push_back(obj->GetComponent<EnemyComponent>());
}

void GameManagerComponent::SpawnPickle()
{
	m_CurrentPickleSpawnCoolDown = m_PickleSpawnCoolDown;
	auto grid = m_pLevel.lock()->GetGrid();
	auto startNode = grid[m_pLevel.lock()->GetPickleSpawn()];
	AnimDurationInit animInit(0.1f, 0.12f, 0.12f, 0.5f, 0.3f);
	auto obj = std::make_shared<dae::GameObject>();
	obj->AddComponent<EnemyComponent>(std::make_shared<EnemyComponent>(obj, 3, startNode, m_pLevel.lock()->GetFloorOffset(), "Textures/Pickle", animInit, 200));
	//obj->GetComponent<dae::Subject>().lock()->AddObserver(m_pPointsObserver);
	m_pScene.lock()->Add(obj, 1);
	m_pPickles.push_back(obj->GetComponent<EnemyComponent>());
}

void GameManagerComponent::InitLevel(size_t index)
{
	if (!m_pLevel.expired())
	{
		for (auto node : m_pLevel.lock()->GetGrid())
			node.second.lock()->GetOwner().lock()->Remove();
		m_pLevel.lock()->GetOwner().lock()->Remove();
	}
	auto levelObject = std::make_shared<dae::GameObject>();
	levelObject->AddComponent<LevelComponent>(std::make_shared<LevelComponent>(levelObject, "../Data/" + m_LevelFolders[index] + "/Level.txt", 3, m_pScene));
	//Adding level to scene
	m_pLevel = levelObject->GetComponent<LevelComponent>();
	//Preparations to center level on screen
	auto size = m_pLevel.lock()->getLevelSize();
	auto window = dae::Renderer::GetInstance().GetWindow();
	int windowWidth, windowHeight;
	SDL_GetWindowSize(window, &windowWidth, &windowHeight);
	glm::vec2 pos;
	//Positioning level to center
	pos.x = static_cast<float>(windowWidth - size.first) / 2.f;
	pos.y = static_cast<float>(windowHeight - size.second) / 2.f;
	levelObject->SetPosition(pos);
	//Setting nodes transformation to level transformation
	m_pLevel.lock()->SetNodeOffset(pos);
	m_pScene.lock()->Add(levelObject, 3);
}

void GameManagerComponent::HandleFallingBurgers()
{
	for (auto burger : m_pBurgers)
	{
		for (auto ingredient : burger.lock()->getIngredients())
		{
			if (ingredient.lock()->hasDropped())
			{
				auto player = ingredient.lock()->GetPlayer();
				auto pos = ingredient.lock()->GetOwner().lock()->GetTransform().GetPosition();
				auto size = ingredient.lock()->GetRectSize();
				pos.x -= size.first / 2;
				Rect ingredientRect{ static_cast<int>(pos.x), static_cast<int>(pos.y), size.first, size.second };
				for (auto hotdog : m_pHotDogs)
				{
					if (ingredient.lock()->GetNode().lock() != hotdog.lock()->getNode().lock())
						continue;
					pos = hotdog.lock()->GetOwner().lock()->GetTransform().GetPosition();
					size = hotdog.lock()->GetRectSize();
					pos.x -= size.first / 2;
					Rect enemyRect{ static_cast<int>(pos.x), static_cast<int>(pos.y), size.first, size.second };
					if (CheckRectOverlap(ingredientRect, enemyRect))
						hotdog.lock()->Kill(player);
				}
				for (auto egg : m_pEggs)
				{
					if (ingredient.lock()->GetNode().lock() != egg.lock()->getNode().lock())
						continue;
					pos = egg.lock()->GetOwner().lock()->GetTransform().GetPosition();
					size = egg.lock()->GetRectSize();
					pos.x -= size.first / 2;
					Rect enemyRect{ static_cast<int>(pos.x), static_cast<int>(pos.y), size.first, size.second };
					if (CheckRectOverlap(ingredientRect, enemyRect))
						egg.lock()->Kill(player);
				}
				for (auto pickle : m_pPickles)
				{
					if (ingredient.lock()->GetNode().lock() != pickle.lock()->getNode().lock())
						continue;
					pos = pickle.lock()->GetOwner().lock()->GetTransform().GetPosition();
					size = pickle.lock()->GetRectSize();
					pos.x -= size.first / 2;
					Rect enemyRect{ static_cast<int>(pos.x), static_cast<int>(pos.y), size.first, size.second };
					if (CheckRectOverlap(ingredientRect, enemyRect))
						pickle.lock()->Kill(player);
				}
			}
		}
	}
}

void GameManagerComponent::HandleSalt()
{
	for (auto player : m_pPlayers)
	{
		if (player.expired())
			return;
		auto salt = player.lock()->GetSalt();
		if (!salt.expired())
		{
			auto pos = salt.lock()->GetOwner().lock()->GetTransform().GetPosition();
			auto size = salt.lock()->GetRectSize();
			pos.x -= size.first / 2;
			Rect saltRect{ static_cast<int>(pos.x), static_cast<int>(pos.y), size.first, size.second };
			for (auto hotdog : m_pHotDogs)
			{
				pos = hotdog.lock()->GetOwner().lock()->GetTransform().GetPosition();
				size = hotdog.lock()->GetRectSize();
				pos.x -= size.first / 2;
				Rect enemyRect{ static_cast<int>(pos.x), static_cast<int>(pos.y), size.first, size.second };
				if (CheckRectOverlap(saltRect, enemyRect))
					hotdog.lock()->Stun();
			}
			for (auto egg : m_pEggs)
			{
				pos = egg.lock()->GetOwner().lock()->GetTransform().GetPosition();
				size = egg.lock()->GetRectSize();
				pos.x -= size.first / 2;
				Rect enemyRect{ static_cast<int>(pos.x), static_cast<int>(pos.y), size.first, size.second };
				if (CheckRectOverlap(saltRect, enemyRect))
					egg.lock()->Stun();
			}
			for (auto pickle : m_pPickles)
			{
				pos = pickle.lock()->GetOwner().lock()->GetTransform().GetPosition();
				size = pickle.lock()->GetRectSize();
				pos.x -= size.first / 2;
				Rect enemyRect{ static_cast<int>(pos.x), static_cast<int>(pos.y), size.first, size.second };
				if (CheckRectOverlap(saltRect, enemyRect))
					pickle.lock()->Stun();
			}
		}
	}
}

void GameManagerComponent::Reset()
{
	for (auto egg : m_pEggs)
		if(!egg.expired())
			egg.lock()->GetOwner().lock()->Remove();
	m_pEggs.clear();
	for (auto hotDog : m_pHotDogs)
		if (!hotDog.expired())
			hotDog.lock()->GetOwner().lock()->Remove();
	m_pHotDogs.clear();
	for (auto pickle : m_pPickles)
		if (!pickle.expired())
			pickle.lock()->GetOwner().lock()->Remove();
	m_pPickles.clear();

	m_pPlayers[0].lock()->Respawn();
}

void GameManagerComponent::InitPlayer(size_t idx, size_t totalPlayers)
{
	//Temp spawn node for player, will be updated later
	auto grid = m_pLevel.lock()->GetGrid();
	auto startNode = grid[m_pLevel.lock()->GetPlayerOneSpawn()];
	if(idx == 1)
		startNode = grid[m_pLevel.lock()->GetPlayerTwoSpawn()];
	float offset = 500.f;

	auto scoreObject = std::make_shared<dae::GameObject>();
	auto font = dae::ResourceManager::GetInstance().LoadFont("emulogic.ttf", 20);
	scoreObject->AddComponent<CounterComponent>(std::make_shared<CounterComponent>(scoreObject, font, 0));
	scoreObject->SetPosition(idx * offset + 0.f, 10.f);
	m_pScene.lock()->Add(scoreObject);
	//m_pPointsObserver = std::make_shared<PointsObserver>(scoreObject->GetComponent<CounterComponent>());

	auto healthObject = std::make_shared<dae::GameObject>();
	healthObject->AddComponent<HealthComponent>(std::make_shared<HealthComponent>(healthObject, 3, 5));
	healthObject->SetPosition(idx * offset + 10.f, 10.f);
	m_pScene.lock()->Add(healthObject);

	auto saltObject = std::make_shared<dae::GameObject>();
	saltObject->AddComponent<SaltDisplayComponent>(std::make_shared<SaltDisplayComponent>(saltObject, 3, 5));
	saltObject->SetPosition(idx * offset + 150.f, 10.f);
	m_pScene.lock()->Add(saltObject);

	auto playerObject = std::make_shared<dae::GameObject>();
	playerObject->AddComponent<PeterPepperComponent>(std::make_shared<PeterPepperComponent>(playerObject, 3, startNode, m_pLevel.lock()->GetFloorOffset(), m_pScene, 5));
	playerObject->GetComponent<dae::Subject>().lock()->AddObserver(std::make_shared<HealthObserver>(healthObject->GetComponent<HealthComponent>()));
	playerObject->GetComponent<dae::Subject>().lock()->AddObserver(std::make_shared<SaltObserver>(saltObject->GetComponent<SaltDisplayComponent>()));
	playerObject->GetComponent<dae::Subject>().lock()->AddObserver(std::make_shared<PointsObserver>(scoreObject->GetComponent<CounterComponent>()));
	m_pPlayers[idx] = playerObject->GetComponent<PeterPepperComponent>();
	if (idx == 0)
	{
		dae::InputManager::GetInstance().AddKeyboardInput('w', dae::InputType::Hold, std::make_shared<CharacterMoveCommand>(m_pPlayers[idx], Action::ClimbingUp));
		dae::InputManager::GetInstance().AddKeyboardInput('s', dae::InputType::Hold, std::make_shared<CharacterMoveCommand>(m_pPlayers[idx], Action::ClimbingDown));
		dae::InputManager::GetInstance().AddKeyboardInput('a', dae::InputType::Hold, std::make_shared<CharacterMoveCommand>(m_pPlayers[idx], Action::WalkingLeft));
		dae::InputManager::GetInstance().AddKeyboardInput('d', dae::InputType::Hold, std::make_shared<CharacterMoveCommand>(m_pPlayers[idx], Action::WalkingRight));
		dae::InputManager::GetInstance().AddKeyboardInput('e', dae::InputType::Hold, std::make_shared<ThrowSaltCommand>(m_pPlayers[idx]));
	}
	if ((idx == 0 && totalPlayers == 1) || idx == 1)
	{
		dae::InputManager::GetInstance().AddControllerInput(0x5820, dae::InputType::Hold, std::make_shared<CharacterMoveCommand>(m_pPlayers[idx], Action::ClimbingUp));
		dae::InputManager::GetInstance().AddControllerInput(0x5821, dae::InputType::Hold, std::make_shared<CharacterMoveCommand>(m_pPlayers[idx], Action::ClimbingDown));
		dae::InputManager::GetInstance().AddControllerInput(0x5823, dae::InputType::Hold, std::make_shared<CharacterMoveCommand>(m_pPlayers[idx], Action::WalkingLeft));
		dae::InputManager::GetInstance().AddControllerInput(0x5822, dae::InputType::Hold, std::make_shared<CharacterMoveCommand>(m_pPlayers[idx], Action::WalkingRight));
	}
	m_pScene.lock()->Add(playerObject);
}

void GameManagerComponent::NextLevel()
{
	++m_CurrentLevel;
	m_CurrentLevel %= m_LevelFolders.size();
	InitLevel(m_CurrentLevel);
	InitBurgers();

	auto grid = m_pLevel.lock()->GetGrid();
	for (auto egg : m_pEggs)
		if (!egg.expired())
			egg.lock()->GetOwner().lock()->Remove();
	m_pEggs.clear();
	for (auto hotDog : m_pHotDogs)
		if (!hotDog.expired())
			hotDog.lock()->GetOwner().lock()->Remove();
	m_pHotDogs.clear();
	for (auto pickle : m_pPickles)
		if (!pickle.expired())
			pickle.lock()->GetOwner().lock()->Remove();
	m_pPickles.clear();

	m_pPlayers[0].lock()->Reset(grid[m_pLevel.lock()->GetPlayerOneSpawn()]);
}

void GameManagerComponent::InitBurgers()
{
	for (auto burger : m_pBurgers)
	{
		for (auto ingredient : burger.lock()->getIngredients())
			ingredient.lock()->GetOwner().lock()->Remove();
		burger.lock()->GetOwner().lock()->Remove();
	}
	m_pBurgers.clear();
	auto grid = m_pLevel.lock()->GetGrid();
	for (auto initData : m_pLevel.lock()->GetBurgerInit())
	{
		auto startNode = grid[{initData[0].idx.first, 0}];
		auto burgerObj = std::make_shared<dae::GameObject>();
		burgerObj->AddComponent<BurgerComponent>(std::make_shared<BurgerComponent>(startNode, m_pScene, initData, burgerObj, 3, m_pLevel, 2));
		m_pBurgers.push_back(burgerObj->GetComponent<BurgerComponent>());
		m_pScene.lock()->Add(burgerObj, 2);
	}
}

std::vector<std::weak_ptr<NodeComponent>> GameManagerComponent::FindPath(std::weak_ptr<NodeComponent> start, std::weak_ptr<NodeComponent> end, std::weak_ptr<NodeComponent> prevNode)
{
	std::queue<std::weak_ptr<NodeComponent>> openList; // Frontier - Expanding edge
	std::map<std::weak_ptr<NodeComponent>, std::weak_ptr<NodeComponent>, std::owner_less<std::weak_ptr<NodeComponent>>> closedList; // Already checked nodes

	openList.push(start); // Kickstarting the loop

	while (!openList.empty())
	{
		std::weak_ptr<NodeComponent> pCurrentNode = openList.front();
		openList.pop();

		if (pCurrentNode.lock() == end.lock())
			break; // EXIT WHILE LOOP

		// Looping over all connections from current node
		for (auto connection : pCurrentNode.lock()->GetConnections())
		{
			if (pCurrentNode.lock() == start.lock() && connection.lock() == prevNode.lock())
				continue;
			std::weak_ptr<NodeComponent> nextNode = connection;
			if (closedList.find(nextNode) == closedList.end())
			{
				openList.push(nextNode);
				closedList[nextNode] = pCurrentNode;
			}
		}
	}

	// Goal node found
	// Track back from goal node to start node to create the path
	std::vector<std::weak_ptr<NodeComponent>> path;
	std::weak_ptr<NodeComponent> pCurrentNode = end; // Start tracking back from end node
	while (pCurrentNode.lock() != start.lock())
	{
		path.push_back(pCurrentNode);
		pCurrentNode = closedList[pCurrentNode];
	}
	path.push_back(pCurrentNode);

	std::reverse(path.begin(), path.end());

	return path;
}
