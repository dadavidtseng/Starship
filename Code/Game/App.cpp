//----------------------------------------------------------------------------------------------------
// App.cpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#include "Game/App.hpp"

#include "Engine/Audio/AudioSystem.hpp"
#include "Engine/Core/Clock.hpp"
#include "Engine/Core/DevConsole.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Math/RandomNumberGenerator.hpp"
#include "Engine/Renderer/BitmapFont.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Platform/Window.hpp"
#include "Game/Game.hpp"

//----------------------------------------------------------------------------------------------------
App*                   g_theApp        = nullptr;   // Created and owned by Main_Windows.cpp
AudioSystem*           g_theAudio      = nullptr;   // Created and owned by the App
BitmapFont*            g_theBitmapFont = nullptr;   // Created and owned by the App
Game*                  g_theGame       = nullptr;   // Created and owned by the App
Renderer*              g_theRenderer   = nullptr;   // Created and owned by the App
RandomNumberGenerator* g_theRNG        = nullptr;   // Created and owned by the App
Window*                g_theWindow     = nullptr;   // Created and owned by the App

//----------------------------------------------------------------------------------------------------
STATIC bool App::m_isQuitting = false;

//----------------------------------------------------------------------------------------------------
void App::Startup()
{
    // Create All Engine Subsystems
    sEventSystemConfig eventSystemConfig;
    g_theEventSystem = new EventSystem(eventSystemConfig);
    g_theEventSystem->SubscribeEventCallbackFunction("OnCloseButtonClicked", OnWindowClose);
    g_theEventSystem->SubscribeEventCallbackFunction("WM_KEYDOWN", Event_KeyPressed);
    g_theEventSystem->SubscribeEventCallbackFunction("quit", OnWindowClose);

    sInputSystemConfig inputConfig;
    g_theInput = new InputSystem(inputConfig);

    sWindowConfig windowConfig;
    windowConfig.m_windowType = eWindowType::FULLSCREEN_CROP;
    windowConfig.m_aspectRatio = 2.f;
    windowConfig.m_inputSystem = g_theInput;
    windowConfig.m_windowTitle = "SD1-A4: Starship Gold";
    g_theWindow                = new Window(windowConfig);

    sRenderConfig renderConfig;
    renderConfig.m_window = g_theWindow;
    g_theRenderer         = new Renderer(renderConfig); // Create render

    // Initialize devConsoleCamera
    m_devConsoleCamera = new Camera();

    sDevConsoleConfig devConsoleConfig;
    devConsoleConfig.m_defaultRenderer = g_theRenderer;
    devConsoleConfig.m_defaultFontName = "SquirrelFixedFont";
    devConsoleConfig.m_defaultCamera   = m_devConsoleCamera;
    g_theDevConsole                    = new DevConsole(devConsoleConfig);

    sAudioSystemConfig audioConfig;
    g_theAudio = new AudioSystem(audioConfig);

    g_theEventSystem->Startup();
    g_theWindow->Startup();
    g_theRenderer->Startup();
    g_theDevConsole->StartUp();
    g_theInput->Startup();
    g_theAudio->Startup();

    g_theBitmapFont = g_theRenderer->CreateOrGetBitmapFontFromFile("Data/Fonts/SquirrelFixedFont"); // DO NOT SPECIFY FILE .EXTENSION!!  (Important later on.)
    g_theRNG        = new RandomNumberGenerator();
    g_theGame       = new Game();
}

//----------------------------------------------------------------------------------------------------
// All Destroy and ShutDown process should be reverse order of the StartUp
//
void App::Shutdown()
{
    delete g_theGame;
    g_theGame = nullptr;

    delete g_theRNG;
    g_theRNG = nullptr;

    delete g_theBitmapFont;
    g_theBitmapFont = nullptr;

    g_theAudio->Shutdown();
    g_theDevConsole->Shutdown();

    delete m_devConsoleCamera;
    m_devConsoleCamera = nullptr;

    g_theRenderer->Shutdown();
    g_theWindow->Shutdown();
    g_theInput->Shutdown();
    g_theEventSystem->Shutdown();

    // Destroy all Engine Subsystem
    delete g_theAudio;
    g_theAudio = nullptr;

    delete g_theRenderer;
    g_theRenderer = nullptr;

    delete g_theWindow;
    g_theWindow = nullptr;

    delete g_theInput;
    g_theInput = nullptr;
}

//----------------------------------------------------------------------------------------------------
// One "frame" of the game.  Generally: Input, Update, Render.  We call this 60+ times per second.
//
void App::RunFrame()
{
    BeginFrame();       // Engine pre-frame stuff
    Update();           // Game updates / moves / spawns / hurts / kills stuff
    Render();           // Game draws current state of things
    EndFrame();         // Engine post-frame stuff
}

//----------------------------------------------------------------------------------------------------
void App::RunMainLoop()
{
    // Program main loop; keep running frames until it's time to quit
    while (!m_isQuitting)
    {
        // Sleep(16); // Temporary code to "slow down" our app to ~60Hz until we have proper frame timing in
        RunFrame();
    }
}

//----------------------------------------------------------------------------------------------------
bool App::OnWindowClose(EventArgs& arg)
{
    UNUSED(arg)

    RequestQuit();
    return true;
}

bool App::Event_KeyPressed(EventArgs& args)
{
    if (g_theDevConsole->IsOpen() == true)
    {
        return false;
    }

    int const           value   = args.GetValue("WM_KEYDOWN", -1);
    unsigned char const keyCode = static_cast<unsigned char>(value);

    if (keyCode == KEYCODE_ESC)
    {
        switch (g_theGame->IsAttractMode())
        {
        case true:
            RequestQuit();

            break;

        case false:
            g_theGame->ResetData();
            g_theApp->DeleteAndCreateNewGame();
            g_theGame->SetAttractMode(true);
            g_theGame->SetPlayerShipIsReadyToSpawnBullet(false);

            break;
        }
    }

    return false;
}

//----------------------------------------------------------------------------------------------------
void App::RequestQuit()
{
    m_isQuitting = true;
}

//----------------------------------------------------------------------------------------------------
void App::BeginFrame() const
{
    g_theEventSystem->BeginFrame();
    g_theInput->BeginFrame();
    g_theWindow->BeginFrame();
    g_theRenderer->BeginFrame();
    g_theDevConsole->BeginFrame();
    g_theAudio->BeginFrame();
    // g_theNetwork->BeginFrame();
}

//----------------------------------------------------------------------------------------------------
void App::Update()
{
    Clock::TickSystemClock();

    HandleKeyPressed();
    HandleKeyReleased();
    AdjustForPauseAndTimeDistortion();
    g_theGame->Update();
}

//----------------------------------------------------------------------------------------------------
// Some simple OpenGL example drawing code.
// This is the graphical equivalent of printing "Hello, world."
//
// Ultimately this function (App::Render) will only call methods on Renderer (like Renderer::DrawVertexArray)
//	to draw things, never calling OpenGL (nor DirectX) functions directly.
//
void App::Render() const
{
    Rgba8 const clearColor = Rgba8::BLACK;

    g_theRenderer->ClearScreen(clearColor);
    g_theGame->Render();

    AABB2 const box = AABB2(Vec2::ZERO, Vec2(1600.f, 30.f));
    Vec2 const bottomLeft     = Vec2::ZERO;
    Vec2 const screenTopRight = Vec2(1600.f, 800.f);
    float      x              = (float)Window::s_mainWindow->GetClientDimensions().x;
    float      y              = (float)Window::s_mainWindow->GetClientDimensions().y;
    m_devConsoleCamera->SetOrthoGraphicView(bottomLeft, screenTopRight);
    m_devConsoleCamera->SetNormalizedViewport(AABB2::ZERO_TO_ONE);
    g_theDevConsole->Render(box);
}

//----------------------------------------------------------------------------------------------------
void App::EndFrame() const
{
    g_theEventSystem->EndFrame();
    g_theInput->EndFrame();
    g_theWindow->EndFrame();
    g_theRenderer->EndFrame();
    g_theDevConsole->EndFrame();
    g_theAudio->EndFrame();
}

//----------------------------------------------------------------------------------------------------
void App::HandleKeyPressed()
{
    if (g_theDevConsole->IsOpen() == true)
    {
        return;
    }

    XboxController const& controller = g_theInput->GetController(0);

    if (g_theInput->WasKeyJustPressed(KEYCODE_O))
    {
        Clock::GetSystemClock().StepSingleFrame();
    }

    if (g_theInput->WasKeyJustPressed('T'))
        m_isSlowMo = true;

    if (g_theInput->WasKeyJustPressed('P'))
        Clock::GetSystemClock().TogglePause();

    if (g_theInput->WasKeyJustPressed(KEYCODE_F4) || controller.WasButtonJustPressed(XBOX_BUTTON_DPAD_DOWN))
    {
        if (g_theGame)
        {
            g_theGame->MarkAllEntityAsDeadAndGarbage();
        }
    }

    if (!g_theGame->IsAttractMode())
    {
        if (g_theInput->WasKeyJustPressed(KEYCODE_F8))
        {
            DeleteAndCreateNewGame();
            g_theGame->SetAttractMode(true);
            g_theGame->SetPlayerShipIsReadyToSpawnBullet(!false);
        }
    }
}

//----------------------------------------------------------------------------------------------------
void App::HandleKeyReleased()
{
    XboxController const& controller = g_theInput->GetController(0);

    if (g_theInput->WasKeyJustReleased('T') || controller.WasButtonJustReleased(XBOX_BUTTON_DPAD_UP))
        m_isSlowMo = false;
}

//----------------------------------------------------------------------------------------------------
void App::HandleQuitRequested()
{
    m_isQuitting = true;
}

//----------------------------------------------------------------------------------------------------
void App::AdjustForPauseAndTimeDistortion() const
{
    if (!g_theGame->IsAttractMode())
    {
        if (m_isSlowMo == true)
        {
            Clock::GetSystemClock().SetTimeScale(0.1f);
        }

        if (
            m_isSlowMo == false)
        {
            Clock::GetSystemClock().SetTimeScale(1.f);
        }
    }
}

//----------------------------------------------------------------------------------------------------
void App::DeleteAndCreateNewGame()
{
    delete g_theGame;
    g_theGame = nullptr;

    g_theGame = new Game();
}
