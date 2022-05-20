#include "MiniginPCH.h"
#include "Minigin.h"
#include <thread>
#include "InputManager.h"
//#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "TextureComponent.h"
#include "TextComponent.h"
#include "GameObject.h"
#include "FpsComponent.h"
#include "MrPepperComponent.h"
#include "HealthObserver.h"
#include "PointsObserver.h"
#include "SoundLocator.h"
#include "SoundLogger.h"
#include "SoundService.h"

#define MAX_LIVES 3

using namespace std;

void PrintSDLVersion()
{
	SDL_version compiled{};
	SDL_version linked{};

	SDL_VERSION(&compiled);
	SDL_GetVersion(&linked);
	printf("We compiled against SDL version %d.%d.%d ...\n",
		compiled.major, compiled.minor, compiled.patch);
	printf("We are linking against SDL version %d.%d.%d.\n",
		linked.major, linked.minor, linked.patch);
}

void dae::Minigin::Initialize()
{
	PrintSDLVersion();
	
	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	m_Window = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		640,
		480,
		SDL_WINDOW_OPENGL
	);
	if (m_Window == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	Renderer::GetInstance().Init(m_Window);
	m_pGameObject = new GameObject();
}

/**
 * Code constructing the scene world starts here
 */
void dae::Minigin::LoadGame() const
{
	auto obj = new TextureComponent();
	obj->SetTexture("background.jpg");

	auto backgroundObject = new GameObject();
	backgroundObject->AddComponent<TextureComponent>(obj);
	m_pGameObject->AddChild(backgroundObject);

	obj = new TextureComponent();
	obj->SetTexture("logo.png");

	auto logoObject = new GameObject();
	logoObject->AddComponent<TextureComponent>(obj);
	logoObject->SetPosition(216, 180);
	m_pGameObject->AddChild(logoObject);

	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	//auto to = new TextComponent("Programming 4 Assignment", font);

	auto textObject = new GameObject();
	//textObject->AddComponent<TextComponent>(to);
	textObject->SetPosition(80, 20);
	m_pGameObject->AddChild(textObject);

	auto comp = new FpsComponent(font);

	auto fpsObject = new GameObject();
	fpsObject->AddComponent<FpsComponent>(comp);
	fpsObject->SetPosition(10, 10);
	m_pGameObject->AddChild(fpsObject);


	SoundLocator::register_sound_system(new SoundService());
	auto& ss1 = SoundLocator::get_sound_system();
	auto t1 = ss1.loadSound("../Audio/SoundEffect/Ammo.wav", SoundType::EFFECT);
	ss1.play(sound_id(t1), 100, SoundType::EFFECT);

	SoundLocator::register_sound_system(new SoundLogger(ss1));
	auto& ss2 = SoundLocator::get_sound_system();
	auto t2 = ss2.loadSound("../Audio/SoundEffect/BigExplosion.wav", SoundType::EFFECT);
	ss2.play(sound_id(t2), 100, SoundType::EFFECT);

	/*auto MrPepperObject = new GameObject();
	auto mrPepperSub = new MrPepper(SDLK_q, SDLK_w, SDLK_e, 3);
	std::cout << "Controls mrPepper:\n";
	std::cout << "Die: q\n";
	std::cout << "Drop Burger: w\n";
	std::cout << "Kill enemy: e\n\n";
	to = new TextComponent("Levens: 3", font);
	HealthObserver* healthObserver = new HealthObserver(3, to);
	mrPepperSub->AddObserver(healthObserver);

	MrPepperObject->AddComponent<TextComponent>(to);
	MrPepperObject->SetPosition(10, 200);

	auto PointsObject = new GameObject();

	to = new TextComponent("Points: 0", font);
	PointsObserver* pointsObserver = new PointsObserver(to);
	mrPepperSub->AddObserver(pointsObserver);
	MrPepperObject->SetSubject(mrPepperSub);

	PointsObject->AddComponent<TextComponent>(to);
	PointsObject->SetPosition(10, 230);
	MrPepperObject->AddChild(PointsObject);

	m_pGameObject->AddChild(MrPepperObject);

	auto MrPepperObject2 = new GameObject();
	auto mrPepperSub2 = new MrPepper(SDLK_a, SDLK_s, SDLK_d, MAX_LIVES);
	std::cout << "Controls mrPepper2:\n";
	std::cout << "Die: a\n";
	std::cout << "Drop Burger: s\n";
	std::cout << "Kill enemy: d\n\n";
	to = new TextComponent("Levens: 3", font);
	HealthObserver* healthObserver2 = new HealthObserver(MAX_LIVES, to);
	mrPepperSub2->AddObserver(healthObserver2);

	MrPepperObject2->AddComponent<TextComponent>(to);
	MrPepperObject2->SetPosition(10, 300);

	auto PointsObject2 = new GameObject();

	to = new TextComponent("Points: 0", font);
	PointsObserver* pointsObserver2 = new PointsObserver(to);
	mrPepperSub2->AddObserver(pointsObserver2);
	MrPepperObject2->SetSubject(mrPepperSub2);

	PointsObject2->AddComponent<TextComponent>(to);
	PointsObject2->SetPosition(10, 330);
	MrPepperObject2->AddChild(PointsObject2);

	m_pGameObject->AddChild(MrPepperObject2);*/
}

void dae::Minigin::Cleanup()
{
	delete m_pGameObject;
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(m_Window);
	m_Window = nullptr;
	SDL_Quit();
}

void dae::Minigin::Run()
{
	Initialize();

	// tell the resource manager where he can find the game data
	ResourceManager::GetInstance().Init("../Data/");

	LoadGame();

	{
		auto& renderer = Renderer::GetInstance();
		//auto& sceneManager = SceneManager::GetInstance();
		auto& input = InputManager::GetInstance();

		// todo: this update loop could use some work.
		bool doContinue = true;
		auto lastTime = std::chrono::high_resolution_clock::now();
		float lag = 0.0f;
		while (doContinue)
		{
			const auto currentTime = std::chrono::high_resolution_clock::now();
			float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
			lastTime = currentTime;
			lag += deltaTime;
			doContinue = input.ProcessInput();
			while (lag * 1000.f >= MsPerFrame)
			{
				m_pGameObject->StaticUpdate(MsPerFrame);
				lag -= MsPerFrame / 1000.f;
			}
			m_pGameObject->Update(deltaTime);
			renderer.Render(m_pGameObject);
			//auto& ss1 = SoundLocator::get_sound_system();
			//ss1.update();
		}
	}

	Cleanup();
}
