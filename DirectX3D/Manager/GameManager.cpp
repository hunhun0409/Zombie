#include "Framework.h"

#include "Scenes/GridScene.h"
#include "Scenes/TerrainScene.h"
#include "Scenes/TerrainEditorScene.h"
#include "Scenes/LightScene.h"
#include "Scenes/CollisionScene.h"
#include "Scenes/MineCraftScene.h"
#include "Scenes/ModelExportScene.h"
#include "Scenes/ModelRenderScene.h"
#include "Scenes/ModelAnimationScene.h"
#include "Scenes/ParticleScene.h"
#include "Scenes/InstancingScene.h"
#include "Scenes/GameScene.h"
#include "Scenes/RenderTargetScene.h"
#include "Scenes/DeferredScene.h"
#include "Scenes/ShadowScene.h"
#include "Scenes/AStarScene.h"
#include "Scenes/SkyScene.h"
#include "Scenes/BillboardScene.h"
#include "Scenes/ZombieScene.h"
#include "Scenes/BuildingEditorScene.h"
#include "Scenes/ParticleToolScene.h"
#include "Scenes/TrailEffectScene.h"
#include "Scenes/ActionScene.h"
#include "Scenes/WeaponScene.h"
#include "Scenes/WaterScene.h"
#include "Scenes/QuadTreeScene.h"
#include "Scenes/TessellationScene.h"
#include "Scenes/TerrainLODScene.h"
#include "Scenes/ScatteringScene.h"

GameManager::GameManager()
{
    Create();

    //Change
    //Laptop, DESKTOP
    //FINAL

    //SceneManager::Get()->Create("Grid", new SkyScene());
    //SceneManager::Get()->Create("Grid", new GridScene());
    //SceneManager::Get()->Create("Export", new ModelExportScene());

    //SceneManager::Get()->Create("Start", new ModelRenderScene());
    //SceneManager::Get()->Create("Start", new ModelAnimationScene());
    //SceneManager::Get()->Create("Start", new BillboardScene());
    //SceneManager::Get()->Create("Start", new ParticleScene());
    //SceneManager::Get()->Create("Start", new ParticleToolScene());

    //SceneManager::Get()->Create("Start", new TerrainEditorScene());
    //SceneManager::Get()->Create("Start", new BuildingEditorScene());
    
    //SceneManager::Get()->Create("Start", new TerrainScene());
    //SceneManager::Get()->Create("Start", new LightScene());
    //SceneManager::Get()->Create("Start", new CollisionScene());
    //SceneManager::Get()->Create("Start", new MineCraftScene());

    //SceneManager::Get()->Create("Start", new ActionScene());
    //SceneManager::Get()->Create("Start", new InstancingScene());
    //SceneManager::Get()->Create("Start", new RenderTargetScene());
    //SceneManager::Get()->Create("Start", new DeferredScene());
    //SceneManager::Get()->Create("Start", new ShadowScene());
    //SceneManager::Get()->Create("Start", new AStarScene());
    
    //SceneManager::Get()->Create("Start", new TrailEffectScene());
    //SceneManager::Get()->Create("Start", new GameScene());
    SceneManager::Get()->Create("Start", new ZombieScene());
    //SceneManager::Get()->Create("Start", new WeaponScene());
    //SceneManager::Get()->Create("Start", new WaterScene());
    //SceneManager::Get()->Create("Start", new QuadTreeScene());
    //SceneManager::Get()->Create("Start", new TessellationScene());
    //SceneManager::Get()->Create("Start", new TerrainLODScene());
    //SceneManager::Get()->Create("Start", new ScatteringScene());

    SceneManager::Get()->Add("Grid");
    SceneManager::Get()->Add("Export");
    SceneManager::Get()->Add("Start");
}

GameManager::~GameManager()
{
    Delete();
}

void GameManager::Update()
{
    Keyboard::Get()->Update();
    Timer::Get()->Update();

    SceneManager::Get()->Update();

    Environment::Get()->Update();

    Audio::Get()->Update();
}

void GameManager::Render()
{
    SceneManager::Get()->PreRender();

    Device::Get()->Clear();
    Font::Get()->GetDC()->BeginDraw();

    Environment::Get()->Set();
    SceneManager::Get()->Render();

    Environment::Get()->PostSet();
    SceneManager::Get()->PostRender();

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    string fps = "FPS : " + to_string(Timer::Get()->GetFPS());
    Font::Get()->RenderText(fps, { 60, WIN_HEIGHT - 50}, {100, 50});

    static bool IsActive = true;

    if (IsActive)
    {
        ImGui::Begin("Inspector", &IsActive);

        Environment::Get()->GUIRender();
        SceneManager::Get()->GUIRender();
        ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    Font::Get()->GetDC()->EndDraw();

    Device::Get()->Present();
}

void GameManager::Create()
{
    Keyboard::Get();
    Timer::Get();
    Device::Get();
    Environment::Get();
    Observer::Get();
    Audio::Get();

    PlayerController::Get();
    ColliderManager::Get();

    Font::Get()->AddColor("White", 1, 1, 1);
    Font::Get()->AddStyle("Default", L"배달의민족 주아");

    Font::Get()->SetColor("White");
    Font::Get()->SetStyle("Default");

    Texture::Add(L"Textures/Color/White.png");

    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(hWnd);
    ImGui_ImplDX11_Init(DEVICE, DC);

    ClientToScreen(hWnd, &clientCenterPos);
}

void GameManager::Delete()
{
    Keyboard::Delete();
    Timer::Delete();
    Device::Delete();
    Shader::Delete();
    Texture::Delete();
    Environment::Delete();
    Observer::Delete();
    Font::Delete();
    SceneManager::Delete();

    InstanceObjectManager::Delete();
    InstanceCharacterManager::Delete();
    ColliderManager::Delete();

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();

    ImGui::DestroyContext();
}
