#include "LevelComponent.h"
#include "TextureManagerComponent.h"
#include "GameObject.h"
#include <fstream>
#include <regex>
#include "Scene.h"

LevelComponent::LevelComponent(const std::shared_ptr<Engine::GameObject>& owner, const std::string& filePath, int scale, const std::weak_ptr<Engine::Scene>& scene)
	:BaseComponent(owner)
{
	ReadFile(filePath, scale, scene);
}

void LevelComponent::Update()
{
}

void LevelComponent::StaticUpdate()
{
}

void LevelComponent::Render(const Engine::Transform&) const
{
}

std::pair<int, int> LevelComponent::getLevelSize()
{
	std::pair<int, int> size{0,0};
	for (int i{ 0 }; i < m_GridSize.first; ++i)
		size.first += m_Grid[{i, 0}].lock()->GetNodeSize().first;
	for (int i{ 0 }; i < m_GridSize.second; ++i)
		size.second += m_Grid[{0, i}].lock()->GetNodeSize().second;
	//int Odd = (int)std::ceil(m_GridSize.first / 2.f);
	//int Even = Odd - m_GridSize.first % 2;
	//size.first = Odd * m_OddTileSize.first + Even * m_EvenTileSize.first;
	//Odd = (int)std::ceil(m_GridSize.second / 2.f);
	//Even = Odd - m_GridSize.second % 2;
	//size.second = Odd * m_OddTileSize.second + Even * m_EvenTileSize.second;
	return size;
}

const std::map<std::pair<int, int>, std::weak_ptr<NodeComponent>> LevelComponent::GetGrid() const
{
	return m_Grid;
}

const std::pair<int, int> LevelComponent::CoordinateToIndex(const glm::vec2& pos)
{
	std::pair<int, int> idx{-1,-1};
	auto botLeft = GetOwner().lock()->GetTransform().GetPosition();
	if (pos.x < botLeft.x || pos.y < botLeft.y)
		return { -1,-1 };
	for (int i = 0; i < m_GridSize.first; ++i)
	{
		auto node = m_Grid[{i, 0}];
		auto nodePos = node.lock()->GetNodePos();
		auto nodeSize = node.lock()->GetNodeSize();
		if (pos.x >= botLeft.x + nodePos.first && pos.x < (botLeft.x + nodePos.first + nodeSize.first))
		{
			idx.first = i;
			break;
		}
	}
	if (idx.first == -1)
		return { -1,-1 };

	for (int i = 0; i < m_GridSize.second; ++i)
	{
		auto node = m_Grid[{0, i}];
		auto nodePos = node.lock()->GetNodePos();
		auto nodeSize = node.lock()->GetNodeSize();
		if (pos.y >= botLeft.y + nodePos.second && pos.y < (botLeft.y + nodePos.second + nodeSize.second))
		{
			idx.second = i;
			break;
		}
	}
	if (idx.second == -1)
		return {-1,-1};
	return idx;
}

const int LevelComponent::GetFloorOffset() const
{
	return m_FloorOffset;
}

std::vector<std::vector<BurgerInit>> LevelComponent::GetBurgerInit() const
{
	return m_BurgerData;
}

const std::pair<int, int> LevelComponent::GetPlayerOneSpawn() const
{
	return m_PlayerOneSpawn;
}

const std::pair<int, int> LevelComponent::GetPlayerTwoSpawn() const
{
	return m_PlayerTwoSpawn;
}

const std::pair<int, int> LevelComponent::GetHotDogSpawn() const
{
	return m_HotDogSpawn;
}

const std::pair<int, int> LevelComponent::GetEggSpawn() const
{
	return m_EggSpawn;
}

const std::pair<int, int> LevelComponent::GetPickleSpawn() const
{
	return m_PickleSpawn;
}

void LevelComponent::SetNodeOffset(const glm::vec2& pos)
{
	for (auto node : m_Grid)
	{
		node.second.lock()->GetOwner().lock()->SetPosition(pos);
	}
}

void LevelComponent::ReadFile(const std::string& filePath, int scale, const std::weak_ptr<Engine::Scene>& scene)
{
	m_PlayerOneSpawn;
	m_PlayerTwoSpawn;
	m_HotDogSpawn;
	m_EggSpawn;
	m_PickleSpawn;

	std::regex playerOneRegex{ "<PlayerOneSpawn>\\s*<(\\d*),(\\d*)>" };
	std::regex playerTwoRegex{ "<PlayerTwoSpawn>\\s*<(\\d*),(\\d*)>" };
	std::regex hotDogRegex{ "<HotDogSpawn>\\s*<(\\d*),(\\d*)>" };
	std::regex eggRegex{ "<EggSpawn>\\s*<(\\d*),(\\d*)>" };
	std::regex pickleRegex{ "<PickleSpawn>\\s*<(\\d*),(\\d*)>" };
	std::regex gridSizeRegex{ "<GridSize>\\s*<(\\d*), (\\d*)>" };
	std::regex oddTileSizeRegex{ "<TileSizeOdd>\\s*<(\\d*), (\\d*)>" };
	std::regex evenTileSizeRegex{ "<TileSizeEven>\\s*<(\\d*), (\\d*)>" };
	std::regex ladderaccessRegex{ "<LadderAccessSize>\\s*<(\\d*), (\\d*)>" };
	std::regex FloorOffsetRegex{ "<FloorOffset>\\s*<(\\d*)>" };
	std::regex nodeRegex{ "<index (\\d*), index (\\d*)>\\s*<(\\d*), (\\d*)>" };
	std::regex VoidTileHeightRegex{ "<TileHeightVoid>\\s*<(\\d*)>" };
	std::regex IngredientComp{ "<(BunTop|Tomato|Patty|Cheese|Lettuce|BunBot)>\\s*<(\\d*),(\\d*)>" };
	int idx{ 0 };
	//std::vector<BurgerInit> initData;
	//initData.push_back(BurgerInit{ {1,1},IngredientType::BunBot });
	std::smatch matches{};
	std::ifstream in{ filePath };
	std::pair<int, int> ladderAccessSize;

	if (!in)
		return;
	
	while (in)
	{
		std::string line;
		std::getline(in, line);
		if (std::regex_match(line, playerOneRegex))
		{
			std::regex_search(line, matches, playerOneRegex);
			m_PlayerOneSpawn.first = std::stoi(matches[1]);
			m_PlayerOneSpawn.second = std::stoi(matches[2]);
		}
		if (std::regex_match(line, playerTwoRegex))
		{
			std::regex_search(line, matches, playerTwoRegex);
			m_PlayerTwoSpawn.first = std::stoi(matches[1]);
			m_PlayerTwoSpawn.second = std::stoi(matches[2]);
		}
		if (std::regex_match(line, hotDogRegex))
		{
			std::regex_search(line, matches, hotDogRegex);
			m_HotDogSpawn.first = std::stoi(matches[1]);
			m_HotDogSpawn.second = std::stoi(matches[2]);
		}
		if (std::regex_match(line, pickleRegex))
		{
			std::regex_search(line, matches, pickleRegex);
			m_PickleSpawn.first = std::stoi(matches[1]);
			m_PickleSpawn.second = std::stoi(matches[2]);
		}
		if (std::regex_match(line, eggRegex))
		{
			std::regex_search(line, matches, eggRegex);
			m_EggSpawn.first = std::stoi(matches[1]);
			m_EggSpawn.second = std::stoi(matches[2]);
		}
		if (std::regex_match(line, VoidTileHeightRegex))
		{
			std::regex_search(line, matches, VoidTileHeightRegex);
			m_TileHeightVoid = std::stoi(matches[1]) * scale;
		}
		if (std::regex_match(line, gridSizeRegex))
		{
			std::regex_search(line, matches, gridSizeRegex);
			m_GridSize.first = std::stoi(matches[1]);
			m_GridSize.second = std::stoi(matches[2]);
		}
		if (std::regex_match(line, oddTileSizeRegex))
		{
			std::regex_search(line, matches, oddTileSizeRegex);
			m_OddTileSize.first = std::stoi(matches[1]) * scale;
			m_OddTileSize.second = std::stoi(matches[2]) * scale;
		}
		if (std::regex_match(line, evenTileSizeRegex))
		{
			std::regex_search(line, matches, evenTileSizeRegex);
			m_EvenTileSize.first = std::stoi(matches[1]) * scale;
			m_EvenTileSize.second = std::stoi(matches[2]) * scale;
		}
		if (std::regex_match(line, ladderaccessRegex))
		{
			std::regex_search(line, matches, ladderaccessRegex);
			ladderAccessSize.first = std::stoi(matches[1]) * scale;
			ladderAccessSize.second = std::stoi(matches[2]) * scale;
		}
		if (std::regex_match(line, FloorOffsetRegex))
		{
			std::regex_search(line, matches, FloorOffsetRegex);
			m_FloorOffset = std::stoi(matches[1]) * scale;
		}
		if (std::regex_match(line, nodeRegex))
		{

			std::regex_search(line, matches, nodeRegex);
			GridNodeInit node;
			node.floor = std::stoi(matches[3]);
			node.ladderAccess = std::stoi(matches[4]);
			int size = m_OddTileSize.first + m_EvenTileSize.first;
			int multiplier = (int)std::floor(std::stoi(matches[1]) / 2.f);
			node.nodePos.first = multiplier * size;
			node.nodePos.first += (std::stoi(matches[1]) % 2) * m_OddTileSize.first;
			size = m_OddTileSize.second + m_EvenTileSize.second;
			//int firstSize = m_TileHeightVoid + m_EvenTileSize.second;
			float mul = (std::stoi(matches[2]) + 1) / 2.f;
			if (std::stoi(matches[2]) > 0)
				node.nodePos.second = m_TileHeightVoid;
			node.nodePos.second += (int)std::ceil(mul) * m_EvenTileSize.second;
			if (std::stoi(matches[2]) > 1)
				node.nodePos.second += ((int)std::floor(mul)-1) * m_OddTileSize.second;
			if (std::stoi(matches[1]) % 2 == 0)
				node.nodeSize.first = m_OddTileSize.first;
			else
				node.nodeSize.first = m_EvenTileSize.first;
			if (std::stoi(matches[2]) == 0)
				node.nodeSize.second = m_TileHeightVoid;
			else if (std::stoi(matches[2]) % 2 == 0)
				node.nodeSize.second = m_OddTileSize.second;
			else
				node.nodeSize.second = m_EvenTileSize.second;

			node.ladderAccessSize = ladderAccessSize;
			auto obj = std::make_shared<Engine::GameObject>();
			obj->AddComponent<NodeComponent>(std::make_shared<NodeComponent>(obj, node));
			auto comp = obj->GetComponent<NodeComponent>();
			scene.lock()->Add(obj);
			m_Grid[std::pair<int, int>(std::stoi(matches[1]), std::stoi(matches[2]))] = comp;
		}
		if (std::regex_match(line, IngredientComp))
		{
			std::regex_search(line, matches, IngredientComp);
			BurgerInit init;
			init.idx.first = std::stoi(matches[2]);
			init.idx.second = std::stoi(matches[3]);
			if(matches[1] == "BunTop")
				init.type = IngredientType::BunTop;
			if (matches[1] == "Tomato")
				init.type = IngredientType::Tomato;
			if (matches[1] == "Patty")
				init.type = IngredientType::Patty;
			if (matches[1] == "Cheese")
				init.type = IngredientType::Cheese;
			if (matches[1] == "Lettuce")
				init.type = IngredientType::Lettuce;
			if (matches[1] == "BunBot")
				init.type = IngredientType::BunBot;
			if (idx != std::stoi(matches[2]))
			{
				std::vector<BurgerInit> i;
				i.push_back(init);
				idx = std::stoi(matches[2]);
				m_BurgerData.push_back(i);
			}
			else
			{
				m_BurgerData[m_BurgerData.size() - 1].push_back(init);
			}
		}
	}
	std::string bl("Textures/Level/BlueLadder.png");
	std::string blf("Textures/Level/BlueLadderFloor.png");
	std::string gl("Textures/Level/GreenLadder.png");
	std::string glf("Textures/Level/GreenLadderFloor.png");
	std::string f("Textures/Level/Floor.png");
	std::string gf("Textures/Level/GreenFloor.png");
	std::vector<std::pair<const std::string, glm::vec2>> textureInfo;
	std::pair<std::string, glm::vec2> info;
	for (auto node : m_Grid)
	{
		//Add connections
		int idxX = node.first.first;
		int idxY = node.first.second;
		if (m_Grid.find({ idxX - 1, idxY }) != m_Grid.end() /*&& m_Grid[{idxX - 1, idxY}].lock()->IsFloor()*/)
			node.second.lock()->SetConnection(m_Grid[{idxX - 1, idxY}], Direction::LEFT);
		if (m_Grid.find({ idxX + 1, idxY }) != m_Grid.end() /* && m_Grid[{idxX + 1, idxY}].lock()->IsFloor()*/)
			node.second.lock()->SetConnection(m_Grid[{idxX + 1, idxY}], Direction::RIGHT);
		if (m_Grid.find({ idxX, idxY - 1 }) != m_Grid.end() /*&& m_Grid[{idxX, idxY - 1}].lock()->HasLadderAccess()*/)
			node.second.lock()->SetConnection(m_Grid[{idxX, idxY - 1}], Direction::DOWN);
		if (m_Grid.find({ idxX, idxY + 1 }) != m_Grid.end() /*&& m_Grid[{idxX, idxY + 1}].lock()->HasLadderAccess()*/)
			node.second.lock()->SetConnection(m_Grid[{idxX, idxY + 1}], Direction::UP);

		//info.second.x = static_cast<float>(multiplier) * static_cast<float>(size);
		//info.second.x += static_cast<float>(node.first.first % 2) * static_cast<float>(m_OddTileSize.first);
		//info.second.x *= scale;

		//size = m_OddTileSize.second + m_EvenTileSize.second;
		//multiplier = (int)std::floor(node.first.second / 2.f);
		//info.second.y = static_cast<float>(multiplier) * static_cast<float>(size);
		//info.second.y += static_cast<float>(node.first.second % 2) * static_cast<float>(m_OddTileSize.second);
		//info.second.y *= scale;
		
		if (node.second.lock()->IsFloor())
		{
			if (node.second.lock()->HasLadderAccess() && node.first.second < m_GridSize.second - 1 && m_Grid[std::pair<int, int>(node.first.first, node.first.second + 1)].lock()->HasLadderAccess())
			{
				if (node.first.first % 2)
					info.first = glf;
				else
					info.first = blf;
			}
			else
			{
				if (node.first.first % 2 == 0)
					info.first = f;
				else
					info.first = gf;
			}
		}
		else
		{
			if (node.second.lock()->HasLadderAccess())
			{
				if (node.first.first % 2)
					info.first = gl;
				else
					info.first = bl;
			}
		}
		info.second.x = static_cast<float>(node.second.lock()->GetNodePos().first);
		info.second.y = static_cast<float>(node.second.lock()->GetNodePos().second);

		if (node.second.lock()->IsFloor() || node.second.lock()->HasLadderAccess())
			textureInfo.push_back(info);

		//m_Grid[node.first].nodePos.first = (int)info.second.x;
		//m_Grid[node.first].nodePos.second = (int)info.second.y;
		//node.second.nodePos.first = (int)info.second.x;
		//node.second.nodePos.second = (int)info.second.y;

		//if (node.first.first % 2 == 0)
		//	m_Grid[node.first].nodeSize.first = m_OddTileSize.first;
		//else
		//	m_Grid[node.first].nodeSize.first = m_EvenTileSize.first;
		//if (node.first.second % 2 == 0)
		//	m_Grid[node.first].nodeSize.second = m_OddTileSize.second;
		//else
		//	m_Grid[node.first].nodeSize.second = m_EvenTileSize.second;
	}
	//owner->AddComponent<dae::AnimationComponent>(std::make_shared<dae::AnimationComponent>(owner, animInitList));
	GetOwner().lock()->AddComponent<Engine::TextureManagerComponent>(std::make_shared<Engine::TextureManagerComponent>(GetOwner().lock(), textureInfo, scale));
}
