#include "GameManagerComponent.h"
#include "Renderer.h"
#include "InputManager.h"
#include "CharacterMoveCommand.h"
#include "ThrowSaltCommand.h"
#include <queue>
#include <iostream>
#include "ElapsedTime.h"

GameManagerComponent::GameManagerComponent(const std::shared_ptr<dae::GameObject> owner, const std::weak_ptr<dae::Scene>& scene)
	: BaseComponent(owner)
	, m_pScene{scene}
	, m_HotDogSpawnCoolDown{8.f}
	, m_CurrentHotDogSpawnCoolDown{ m_HotDogSpawnCoolDown }
	, m_PickleSpawnCoolDown{ 15.f }
	, m_CurrentPickleSpawnCoolDown{ m_PickleSpawnCoolDown }
	, m_EggSpawnCoolDown{ 15.f }
	, m_currentEggSpawnCoolDown{ m_EggSpawnCoolDown }
{
	InitSinglePlayer();
}

void GameManagerComponent::Update()
{
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
	auto window = dae::Renderer::GetInstance().GetWindow();
	int windowWidth;
	int windowHeight;
	SDL_GetWindowSize(window, &windowWidth, &windowHeight);
	auto node = m_TempPath[0].lock();
	auto trans = node->GetOwner().lock()->GetTransform().GetPosition();
	int x1{ node->GetNodePos().first + node->GetNodeSize().first / 2 + (int)trans.x};
	int y1{ windowHeight - (node->GetNodePos().second + node->GetNodeSize().second / 2 + (int)trans.y) };
	for (size_t i = 1; i < m_TempPath.size(); ++i)
	{
		SDL_SetRenderDrawColor(dae::Renderer::GetInstance().GetSDLRenderer(), 255, 255, 255, 1);
		node = m_TempPath[i].lock();
		int x2{ node->GetNodePos().first + node->GetNodeSize().first / 2 + (int)trans.x };
		int y2{ windowHeight - (node->GetNodePos().second + node->GetNodeSize().second / 2 + (int)trans.y) };
		SDL_RenderDrawLine(dae::Renderer::GetInstance().GetSDLRenderer(), x1, y1, x2, y2);
		x1 = x2;
		y1 = y2;
	}
}

void GameManagerComponent::DeterminePathEnemies(std::vector<std::weak_ptr<EnemyComponent>>& enemies)
{
	for (auto enemy : enemies)
	{
		if (m_pPlayer.expired())
			continue;
		if (!enemy.lock()->ReachedChoicePoint())
			continue;
		m_TempPath = FindPath(enemy.lock()->getNode(), m_pPlayer.lock()->getNode(), enemy.lock()->getPrevNode());
		//std::cout << hotDog.lock()->getNode().lock()->GetNodePos().first << " " << hotDog.lock()->getPrevNode().lock()->GetNodePos().first << "\n";
		if (m_TempPath.size() >= 2)
			enemy.lock()->Move(m_TempPath[1]);
		else
		{
			for (auto node : m_pPlayer.lock()->getNode().lock()->GetConnections())
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
	//Initialize level component
	auto levelObject = std::make_shared<dae::GameObject>();
	levelObject->AddComponent<LevelComponent>(std::make_shared<LevelComponent>(levelObject, "../Data/Level.txt", 3, m_pScene));
	m_pLevel = levelObject->GetComponent<LevelComponent>();
	//Preparations to center level on screen
	auto size = m_pLevel.lock()->getLevelSize();
	auto window = dae::Renderer::GetInstance().GetWindow();
	int windowWidth;
	int windowHeight;
	SDL_GetWindowSize(window, &windowWidth, &windowHeight);
	glm::vec2 pos;
	//Positioning level to center
	pos.x = static_cast<float>(windowWidth - size.first) / 2.f;
	pos.y = static_cast<float>(windowHeight - size.second) / 2.f;
	levelObject->SetPosition(pos);
	//Adding level to scene
	//Setting nodes transformation to level transformation
	m_pLevel.lock()->SetNodeOffset(pos);
	m_pScene.lock()->Add(levelObject);

	//Temp spawn node for player, will be updated later
	auto grid = m_pLevel.lock()->GetGrid();
	auto startNode = grid[{0, 0}];

	auto playerObject = std::make_shared<dae::GameObject>();
	playerObject->AddComponent<PeterPepperComponent>(std::make_shared<PeterPepperComponent>(playerObject, 3, startNode, m_pLevel.lock()->GetFloorOffset(), m_pScene, 5));
	m_pPlayer = playerObject->GetComponent<PeterPepperComponent>();
	dae::InputManager::GetInstance().AddKeyboardInput('w', dae::InputType::Hold, std::make_shared<CharacterMoveCommand>(m_pPlayer, Action::ClimbingUp));
	dae::InputManager::GetInstance().AddKeyboardInput('s', dae::InputType::Hold, std::make_shared<CharacterMoveCommand>(m_pPlayer, Action::ClimbingDown));
	dae::InputManager::GetInstance().AddKeyboardInput('a', dae::InputType::Hold, std::make_shared<CharacterMoveCommand>(m_pPlayer, Action::WalkingLeft));
	dae::InputManager::GetInstance().AddKeyboardInput('d', dae::InputType::Hold, std::make_shared<CharacterMoveCommand>(m_pPlayer, Action::WalkingRight));
	dae::InputManager::GetInstance().AddKeyboardInput('e', dae::InputType::Hold, std::make_shared<ThrowSaltCommand>(m_pPlayer));
	dae::InputManager::GetInstance().AddControllerInput(0x5820, dae::InputType::Hold, std::make_shared<CharacterMoveCommand>(m_pPlayer, Action::ClimbingUp));
	dae::InputManager::GetInstance().AddControllerInput(0x5821, dae::InputType::Hold, std::make_shared<CharacterMoveCommand>(m_pPlayer, Action::ClimbingDown));
	dae::InputManager::GetInstance().AddControllerInput(0x5823, dae::InputType::Hold, std::make_shared<CharacterMoveCommand>(m_pPlayer, Action::WalkingLeft));
	dae::InputManager::GetInstance().AddControllerInput(0x5822, dae::InputType::Hold, std::make_shared<CharacterMoveCommand>(m_pPlayer, Action::WalkingRight));

	std::vector<std::shared_ptr<dae::GameObject>> enemyComponents;
	startNode = grid[{8, 0}];
	AnimDurationInit animInit(0.25f, 0.25f, 0.25f, 0.5f, 0.3f);
	enemyComponents.push_back(std::make_shared<dae::GameObject>());
	enemyComponents.back()->AddComponent<EnemyComponent>(std::make_shared<EnemyComponent>(enemyComponents.back(), 3, startNode, m_pLevel.lock()->GetFloorOffset(), "Textures/HotDog", animInit));
	m_pHotDogs.push_back(enemyComponents.back()->GetComponent<EnemyComponent>());
	enemyComponents.push_back(std::make_shared<dae::GameObject>());
	startNode = grid[{8, 18}];
	enemyComponents.back()->AddComponent<EnemyComponent>(std::make_shared<EnemyComponent>(enemyComponents.back(), 3, startNode, m_pLevel.lock()->GetFloorOffset(), "Textures/Egg", animInit));
	m_pEggs.push_back(enemyComponents.back()->GetComponent<EnemyComponent>());
	startNode = grid[{0, 18}];
	animInit.walk = 0.1f;
	animInit.up = 0.12f;
	animInit.down = 0.12f;
	enemyComponents.push_back(std::make_shared<dae::GameObject>());
	enemyComponents.back()->AddComponent<EnemyComponent>(std::make_shared<EnemyComponent>(enemyComponents.back(), 3, startNode, m_pLevel.lock()->GetFloorOffset(), "Textures/Pickle", animInit));
	m_pPickles.push_back(enemyComponents.back()->GetComponent<EnemyComponent>());

	std::vector<BurgerInit> initData;
	initData.push_back(BurgerInit{ {1,0},IngredientType::BunBot });
	initData.push_back(BurgerInit{ {1,4},IngredientType::Lettuce });
	initData.push_back(BurgerInit{ {1,10},IngredientType::Patty });
	initData.push_back(BurgerInit{ {1,14},IngredientType::Tomato });
	initData.push_back(BurgerInit{ {1,18},IngredientType::BunTop });

	std::vector<std::shared_ptr<dae::GameObject>> burgerComponents;
	burgerComponents.push_back(std::make_shared<dae::GameObject>());
	burgerComponents.back()->AddComponent<BurgerComponent>(std::make_shared<BurgerComponent>(m_pScene, initData, burgerComponents.back(), 3, m_pLevel));
	m_pBurgers.push_back(burgerComponents.back()->GetComponent<BurgerComponent>());
	initData.clear();
	initData.push_back(BurgerInit{ {3,0},IngredientType::BunBot });
	initData.push_back(BurgerInit{ {3,4},IngredientType::Tomato });
	initData.push_back(BurgerInit{ {3,8},IngredientType::Patty });
	initData.push_back(BurgerInit{ {3,12},IngredientType::Cheese });
	initData.push_back(BurgerInit{ {3,18},IngredientType::BunTop });

	burgerComponents.push_back(std::make_shared<dae::GameObject>());
	burgerComponents.back()->AddComponent<BurgerComponent>(std::make_shared<BurgerComponent>(m_pScene, initData, burgerComponents.back(), 3, m_pLevel));
	m_pBurgers.push_back(burgerComponents.back()->GetComponent<BurgerComponent>());
	initData.clear();
	initData.push_back(BurgerInit{ {5,0},IngredientType::BunBot });
	initData.push_back(BurgerInit{ {5,4},IngredientType::Lettuce });
	initData.push_back(BurgerInit{ {5,8},IngredientType::Cheese });
	initData.push_back(BurgerInit{ {5,14},IngredientType::Patty });
	initData.push_back(BurgerInit{ {5,18},IngredientType::BunTop });

	burgerComponents.push_back(std::make_shared<dae::GameObject>());
	burgerComponents.back()->AddComponent<BurgerComponent>(std::make_shared<BurgerComponent>(m_pScene, initData, burgerComponents.back(), 3, m_pLevel));
	m_pBurgers.push_back(burgerComponents.back()->GetComponent<BurgerComponent>());
	initData.clear();
	initData.push_back(BurgerInit{ {7,0},IngredientType::BunBot });
	initData.push_back(BurgerInit{ {7,6},IngredientType::Patty });
	initData.push_back(BurgerInit{ {7,10},IngredientType::Cheese });
	initData.push_back(BurgerInit{ {7,14},IngredientType::Patty });
	initData.push_back(BurgerInit{ {7,18},IngredientType::BunTop });

	burgerComponents.push_back(std::make_shared<dae::GameObject>());
	burgerComponents.back()->AddComponent<BurgerComponent>(std::make_shared<BurgerComponent>(m_pScene, initData, burgerComponents.back(), 3, m_pLevel));
	m_pBurgers.push_back(burgerComponents.back()->GetComponent<BurgerComponent>());
	for(auto obj : burgerComponents)
		m_pScene.lock()->Add(obj);
	for(auto enemy: enemyComponents)
		m_pScene.lock()->Add(enemy);
	m_pScene.lock()->Add(playerObject);

	//m_TempPath = FindPath(grid[{0, 0}], grid[{8, 18}]);
	/*std::pair<int, int> idx{ 0,0 };
	auto prevPos{ grid[idx].lock()->GetNodePos() };
	for (auto node : m_TempPath)
	{
		auto curPos = node.lock()->GetNodePos();
		if (prevPos.first != curPos.first)
		{
			if (prevPos.first > curPos.first)
				--idx.first;
			else
				++idx.first;
		}
		if (prevPos.second != curPos.second)
		{
			if (prevPos.second > curPos.second)
				--idx.second;
			else
				++idx.second;
		}
		std::cout << "x: " << idx.first << ", y: " << idx.second << "\n";
		prevPos = node.lock()->GetNodePos();
	}*/
}

void GameManagerComponent::CheckBurgerOverlap()
{
	if (m_pPlayer.expired())
		return;
	for (auto burger : m_pBurgers)
	{
		for (auto ingredient : burger.lock()->getIngredients())
		{
			bool wasDropped = ingredient.lock()->hasDropped();
			if (ingredient.lock()->CheckOverlap(m_pPlayer, 0) && !wasDropped)
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
						enemy.lock()->Kill();
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
						enemy.lock()->Kill();
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
						enemy.lock()->Kill();
						++extraDrops;
					}
				}
				//int amount = rand() % 3;
				ingredient.lock()->SetExtraDrops(extraDrops);
				//std::cout << amount << "\n";
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
	auto startNode = grid[{8, 0}];
	AnimDurationInit animInit(0.25f, 0.25f, 0.25f, 0.5f, 0.3f);
	auto obj = std::make_shared<dae::GameObject>();
	obj->AddComponent<EnemyComponent>(std::make_shared<EnemyComponent>(obj, 3, startNode, m_pLevel.lock()->GetFloorOffset(), "Textures/HotDog", animInit));
	m_pScene.lock()->Add(obj);
	m_pHotDogs.push_back(obj->GetComponent<EnemyComponent>());
}

void GameManagerComponent::SpawnEgg()
{
	m_currentEggSpawnCoolDown = m_EggSpawnCoolDown;
	auto grid = m_pLevel.lock()->GetGrid();
	auto startNode = grid[{8, 18}];
	AnimDurationInit animInit(0.25f, 0.25f, 0.25f, 0.5f, 0.3f);
	auto obj = std::make_shared<dae::GameObject>();
	obj->AddComponent<EnemyComponent>(std::make_shared<EnemyComponent>(obj, 3, startNode, m_pLevel.lock()->GetFloorOffset(), "Textures/Egg", animInit));
	m_pScene.lock()->Add(obj);
	m_pEggs.push_back(obj->GetComponent<EnemyComponent>());
}

void GameManagerComponent::SpawnPickle()
{
	m_CurrentPickleSpawnCoolDown = m_PickleSpawnCoolDown;
	auto grid = m_pLevel.lock()->GetGrid();
	auto startNode = grid[{0, 18}];
	AnimDurationInit animInit(0.1f, 0.12f, 0.12f, 0.5f, 0.3f);
	auto obj = std::make_shared<dae::GameObject>();
	obj->AddComponent<EnemyComponent>(std::make_shared<EnemyComponent>(obj, 3, startNode, m_pLevel.lock()->GetFloorOffset(), "Textures/Pickle", animInit));
	m_pScene.lock()->Add(obj);
	m_pPickles.push_back(obj->GetComponent<EnemyComponent>());
}

void GameManagerComponent::HandleFallingBurgers()
{
	for (auto burger : m_pBurgers)
	{
		for (auto ingredient : burger.lock()->getIngredients())
		{
			if (ingredient.lock()->hasDropped())
			{
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
						hotdog.lock()->Kill();
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
						egg.lock()->Kill();
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
						pickle.lock()->Kill();
				}
			}
		}
	}
}

void GameManagerComponent::HandleSalt()
{
	auto salt = m_pPlayer.lock()->GetSalt();
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
