#include "LevelComponent.h"
#include "TextureManagerComponent.h"
#include "GameObject.h"
#include <fstream>
#include <regex>

LevelComponent::LevelComponent(const std::shared_ptr<dae::GameObject>& owner, const std::string& filePath, int scale)
	:BaseComponent(owner)
{
	ReadFile(filePath, scale);
}

void LevelComponent::Update()
{
}

void LevelComponent::StaticUpdate()
{
}

void LevelComponent::Render(const dae::Transform&) const
{
}

std::pair<int, int> LevelComponent::getLevelSize()
{
	std::pair<int, int> size;
	int Odd = (int)std::ceil(m_GridSize.first / 2.f);
	int Even = Odd - m_GridSize.first % 2;
	size.first = Odd * m_OddTileSize.first + Even * m_EvenTileSize.first;
	Odd = (int)std::ceil(m_GridSize.second / 2.f);
	Even = Odd - m_GridSize.second % 2;
	size.second = Odd * m_OddTileSize.second + Even * m_EvenTileSize.second;
	return size;
}

const std::map<std::pair<int, int>, GridNode> LevelComponent::GetGrid() const
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
		if (pos.x >= botLeft.x + node.nodePos.first && pos.x < (botLeft.x + node.nodePos.first + node.nodeSize.first))
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
		if (pos.y >= botLeft.y + node.nodePos.second && pos.y < (botLeft.y + node.nodePos.second + node.nodeSize.second))
		{
			idx.second = i;
			break;
		}
	}
	if (idx.second == -1)
		return {-1,-1};
	return idx;
}

void LevelComponent::ReadFile(const std::string& filePath, int scale)
{
	std::regex gridSizeRegex{ "<GridSize>\\s*<(\\d*), (\\d*)>" };
	std::regex oddTileSizeRegex{ "<TileSizeOdd>\\s*<(\\d*), (\\d*)>" };
	std::regex evenTileSizeRegex{ "<TileSizeEven>\\s*<(\\d*), (\\d*)>" };
	std::regex nodeRegex{ "<index (\\d*), index (\\d*)>\\s*<(\\d*), (\\d*)>" };
	std::smatch matches{};
	std::ifstream in{ filePath };
	if (!in)
		return;
	
	while (in)
	{
		std::string line;
		std::getline(in, line);
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
		if (std::regex_match(line, nodeRegex))
		{
			std::regex_search(line, matches, nodeRegex);
			GridNode node;
			node.floor = std::stoi(matches[3]);
			node.ladderAccess = std::stoi(matches[4]);
			m_Grid[std::pair<int, int>(std::stoi(matches[1]), std::stoi(matches[2]))] = node;
		}
	}
	std::string bl("BlueLadder.png");
	std::string blf("BlueLadderFloor.png");
	std::string gl("GreenLadder.png");
	std::string glf("GreenLadderFloor.png");
	std::string f("Floor.png");
	std::string gf("GreenFloor.png");
	std::vector<std::pair<const std::string, glm::vec2>> textureInfo;
	std::pair<std::string, glm::vec2> info;
	for (auto node : m_Grid)
	{
		int size = m_OddTileSize.first + m_EvenTileSize.first;
		int multiplier = (int)std::floor(node.first.first / 2.f);

		info.second.x = static_cast<float>(multiplier) * static_cast<float>(size);
		info.second.x += static_cast<float>(node.first.first % 2) * static_cast<float>(m_OddTileSize.first);
		//info.second.x *= scale;

		size = m_OddTileSize.second + m_EvenTileSize.second;
		multiplier = (int)std::floor(node.first.second / 2.f);
		info.second.y = static_cast<float>(multiplier) * static_cast<float>(size);
		info.second.y += static_cast<float>(node.first.second % 2) * static_cast<float>(m_OddTileSize.second);
		//info.second.y *= scale;
		
		if (node.second.floor == 1)
		{
			if (node.second.ladderAccess == 1 && node.first.second < m_GridSize.second - 1 && m_Grid[std::pair<int, int>(node.first.first, node.first.second + 1)].ladderAccess)
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
			if (node.second.ladderAccess == 1)
			{
				if (node.first.first % 2)
					info.first = gl;
				else
					info.first = bl;
			}
		}

		if (node.second.floor == 1 || node.second.ladderAccess == 1)
			textureInfo.push_back(info);

		m_Grid[node.first].nodePos.first = (int)info.second.x;
		m_Grid[node.first].nodePos.second = (int)info.second.y;
		//node.second.nodePos.first = (int)info.second.x;
		//node.second.nodePos.second = (int)info.second.y;

		if (node.first.first % 2 == 0)
			m_Grid[node.first].nodeSize.first = m_OddTileSize.first;
		else
			m_Grid[node.first].nodeSize.first = m_EvenTileSize.first;
		if (node.first.second % 2 == 0)
			m_Grid[node.first].nodeSize.second = m_OddTileSize.second;
		else
			m_Grid[node.first].nodeSize.second = m_EvenTileSize.second;
	}
	//owner->AddComponent<dae::AnimationComponent>(std::make_shared<dae::AnimationComponent>(owner, animInitList));
	GetOwner().lock()->AddComponent<dae::TextureManagerComponent>(std::make_shared<dae::TextureManagerComponent>(GetOwner().lock(), textureInfo, scale));
}
