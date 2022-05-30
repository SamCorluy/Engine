#include "GameManagerComponent.h"
#include "Renderer.h"
#include "InputManager.h"
#include "CharacterMoveCommand.h"
#include <queue>
#include <iostream>

GameManagerComponent::GameManagerComponent(const std::shared_ptr<dae::GameObject> owner, const std::weak_ptr<dae::Scene>& scene)
	: BaseComponent(owner)
	, m_pScene{scene}
{
	InitSinglePlayer();
}

void GameManagerComponent::Update()
{
	CheckBurgerOverlap();
	auto grid = m_pLevel.lock()->GetGrid();
	//m_TempPath = FindPath(grid[{0, 0}], m_pPlayer.lock()->getNode());
	for (auto enemy: m_pEnemies)
	{
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
	playerObject->AddComponent<PeterPepperComponent>(std::make_shared<PeterPepperComponent>(playerObject, 3, startNode, m_pLevel.lock()->GetFloorOffset()));
	m_pPlayer = playerObject->GetComponent<PeterPepperComponent>();
	dae::InputManager::GetInstance().AddKeyboardInput('w', dae::InputType::Hold, std::make_shared<CharacterMoveCommand>(m_pPlayer, Action::ClimbingUp));
	dae::InputManager::GetInstance().AddKeyboardInput('s', dae::InputType::Hold, std::make_shared<CharacterMoveCommand>(m_pPlayer, Action::ClimbingDown));
	dae::InputManager::GetInstance().AddKeyboardInput('a', dae::InputType::Hold, std::make_shared<CharacterMoveCommand>(m_pPlayer, Action::WalkingLeft));
	dae::InputManager::GetInstance().AddKeyboardInput('d', dae::InputType::Hold, std::make_shared<CharacterMoveCommand>(m_pPlayer, Action::WalkingRight));
	dae::InputManager::GetInstance().AddControllerInput(0x5820, dae::InputType::Hold, std::make_shared<CharacterMoveCommand>(m_pPlayer, Action::ClimbingUp));
	dae::InputManager::GetInstance().AddControllerInput(0x5821, dae::InputType::Hold, std::make_shared<CharacterMoveCommand>(m_pPlayer, Action::ClimbingDown));
	dae::InputManager::GetInstance().AddControllerInput(0x5823, dae::InputType::Hold, std::make_shared<CharacterMoveCommand>(m_pPlayer, Action::WalkingLeft));
	dae::InputManager::GetInstance().AddControllerInput(0x5822, dae::InputType::Hold, std::make_shared<CharacterMoveCommand>(m_pPlayer, Action::WalkingRight));

	std::vector<std::shared_ptr<dae::GameObject>> enemyComponents;
	startNode = grid[{8, 0}];
	enemyComponents.push_back(std::make_shared<dae::GameObject>());
	enemyComponents.back()->AddComponent<EnemyComponent>(std::make_shared<EnemyComponent>(enemyComponents.back(), 3, startNode, m_pLevel.lock()->GetFloorOffset(), "Textures/HotDog"));
	m_pEnemies.push_back(enemyComponents.back()->GetComponent<EnemyComponent>());
	enemyComponents.push_back(std::make_shared<dae::GameObject>());
	startNode = grid[{0, 18}];
	enemyComponents.back()->AddComponent<EnemyComponent>(std::make_shared<EnemyComponent>(enemyComponents.back(), 3, startNode, m_pLevel.lock()->GetFloorOffset(), "Textures/Pickle"));
	m_pEnemies.push_back(enemyComponents.back()->GetComponent<EnemyComponent>());
	enemyComponents.push_back(std::make_shared<dae::GameObject>());
	startNode = grid[{8, 18}];
	enemyComponents.back()->AddComponent<EnemyComponent>(std::make_shared<EnemyComponent>(enemyComponents.back(), 3, startNode, m_pLevel.lock()->GetFloorOffset(), "Textures/Egg"));
	m_pEnemies.push_back(enemyComponents.back()->GetComponent<EnemyComponent>());

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
	for(auto hotdog: enemyComponents)
		m_pScene.lock()->Add(hotdog);
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
	for (auto burger : m_pBurgers)
	{
		for (auto ingredient : burger.lock()->getIngredients())
		{
			ingredient.lock()->CheckOverlap(m_pPlayer);
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
