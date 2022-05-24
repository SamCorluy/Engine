#include "LevelComponent.h"
#include "TextureManagerComponent.h"
#include "GameObject.h"
#include <fstream>
#include <regex>

LevelComponent::LevelComponent(const std::shared_ptr<dae::GameObject>& owner, const std::string& filePath)
	:BaseComponent(owner)
{
	ReadFile(filePath);
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

void LevelComponent::ReadFile(const std::string& filePath)
{
	std::regex gridSizeRegex{ "<GridSize>\\s*<(\\d*), (\\d*)>" };
	std::regex tileSizeRegex{ "<TileSize>\\s*<(\\d*), (\\d*)>" };
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
		if (std::regex_match(line, tileSizeRegex))
		{
			std::regex_search(line, matches, tileSizeRegex);
			m_TileSize.first = std::stoi(matches[1]);
			m_TileSize.second = std::stoi(matches[2]);
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
	std::vector<std::pair<const std::string, glm::vec2>> textureInfo;
	std::pair<std::string, glm::vec2> info;
	for (auto node : m_Grid)
	{
		info.second.x = static_cast<float>(node.first.first) * static_cast<float>(m_TileSize.first);
		info.second.y = static_cast<float>(node.first.second) * static_cast<float>(m_TileSize.second);
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
				info.first = f;
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
	}
	//owner->AddComponent<dae::AnimationComponent>(std::make_shared<dae::AnimationComponent>(owner, animInitList));
	GetOwner().lock()->AddComponent<dae::TextureManagerComponent>(std::make_shared<dae::TextureManagerComponent>(GetOwner().lock(), textureInfo));
}
