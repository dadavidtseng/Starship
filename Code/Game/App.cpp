//----------------------------------------------------------------------------------------------------
// App.cpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#include "Game/App.hpp"

#include "Engine/Audio/AudioSystem.hpp"
#include "Engine/Core/Clock.hpp"
#include "Engine/Core/DevConsole.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/Time.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Math/RandomNumberGenerator.hpp"
#include "Engine/Renderer/BitmapFont.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Renderer/Window.hpp"
#include "Game/Game.hpp"

//----------------------------------------------------------------------------------------------------
App*                   g_theApp        = nullptr;   // Created and owned by Main_Windows.cpp
AudioSystem*           g_theAudio      = nullptr;   // Created and owned by the App
BitmapFont*            g_theBitmapFont = nullptr;   // Created and owned by the App
Renderer*              g_theRenderer   = nullptr;   // Created and owned by the App
RandomNumberGenerator* g_theRNG        = nullptr;   // Created and owned by the App
Window*                g_theWindow     = nullptr;   // Created and owned by the App

//----------------------------------------------------------------------------------------------------
STATIC bool App::m_isQuitting = false;

//----------------------------------------------------------------------------------------------------
void App::Startup()
{
    // Create All Engine Subsystems
    EventSystemConfig eventSystemConfig;
    g_theEventSystem = new EventSystem(eventSystemConfig);
    g_theEventSystem->SubscribeEventCallbackFunction("WM_CLOSE", OnWindowClose);
    g_theEventSystem->SubscribeEventCallbackFunction("WM_KEYDOWN", Event_KeyPressed);
    g_theEventSystem->SubscribeEventCallbackFunction("quit", OnWindowClose);

    InputSystemConfig inputConfig;
    g_theInput = new InputSystem(inputConfig);

    WindowConfig windowConfig;
    windowConfig.m_aspectRatio = 2.f;
    windowConfig.m_inputSystem = g_theInput;
    windowConfig.m_windowTitle = "SD1-A4: Starship Gold";
    g_theWindow                = new Window(windowConfig);

    RenderConfig renderConfig;
    renderConfig.m_window = g_theWindow;
    g_theRenderer         = new Renderer(renderConfig); // Create render

    // Initialize devConsoleCamera
    m_devConsoleCamera = new Camera();

    Vec2 const bottomLeft     = Vec2::ZERO;
    Vec2 const screenTopRight = Vec2(SCREEN_SIZE_X, SCREEN_SIZE_Y);

    m_devConsoleCamera->SetOrthoView(bottomLeft, screenTopRight);

    DevConsoleConfig devConsoleConfig;
    devConsoleConfig.m_defaultRenderer = g_theRenderer;
    devConsoleConfig.m_defaultFontName = "SquirrelFixedFont";
    devConsoleConfig.m_defaultCamera   = m_devConsoleCamera;
    g_theDevConsole                    = new DevConsole(devConsoleConfig);

    AudioSystemConfig audioConfig;
    g_theAudio = new AudioSystem(audioConfig);

    g_theEventSystem->Startup();
    g_theWindow->Startup();
    g_theRenderer->Startup();
    g_theDevConsole->StartUp();
    g_theInput->Startup();
    g_theAudio->Startup();

    g_theBitmapFont = g_theRenderer->CreateOrGetBitmapFontFromFile("Data/Fonts/SquirrelFixedFont"); // DO NOT SPECIFY FILE .EXTENSION!!  (Important later on.)
    g_theRNG  = new RandomNumberGenerator();
    m_theGame = new Game();
}

//----------------------------------------------------------------------------------------------------
// All Destroy and ShutDown process should be reverse order of the StartUp
//
void App::Shutdown()
{
    delete m_theGame;
    m_theGame = nullptr;

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
    float const timeNow      = static_cast<float>(GetCurrentTimeSeconds());
    float const deltaSeconds = timeNow - m_timeLastFrameStart;
    m_timeLastFrameStart     = timeNow;

    // DebuggerPrintf("TimeNow = %.06f\n", timeNow);

    BeginFrame();         // Engine pre-frame stuff
    Update(deltaSeconds); // Game updates / moves / spawns / hurts / kills stuff
    Render();             // Game draws current state of things
    EndFrame();           // Engine post-frame stuff
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
void App::Update(float deltaSeconds)
{
    Clock::TickSystemClock();

    HandleKeyPressed();
    HandleKeyReleased();
    AdjustForPauseAndTimeDistortion(deltaSeconds);
    m_theGame->Update(deltaSeconds);
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
    // g_theRenderer->BeginCamera(*m_devConsoleCamera);
    //
    // DebugDrawRing(Vec2(800.f, 400.f), 300.f, 10.f, Rgba8(255, 127, 0));
    //
    m_theGame->Render();
    // g_theRenderer->EndCamera(*m_devConsoleCamera);
    AABB2 const box = AABB2(Vec2::ZERO, Vec2(1600.f, 30.f));

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
    XboxController const& controller = g_theInput->GetController(0);

    if (g_theInput->WasKeyJustPressed(KEYCODE_O))
    {
        m_isPaused = true;
        m_theGame->Update(1.f / 60.f);
    }

    if (g_theInput->WasKeyJustPressed('T'))
        m_isSlowMo = true;

    if (g_theInput->WasKeyJustPressed('P'))
        m_isPaused = !m_isPaused;

    if (g_theInput->WasKeyJustPressed(KEYCODE_ESC) || controller.WasButtonJustPressed(XBOX_BUTTON_BACK))
    {
        switch (m_theGame->IsAttractMode())
        {
        case true:
            m_isQuitting = true;

            break;

        case false:
            m_theGame->ResetData();
            DeleteAndCreateNewGame();
            m_theGame->SetAttractMode(true);
            m_theGame->SetPlayerShipIsReadyToSpawnBullet(false);

            break;
        }
    }

    if (g_theInput->WasKeyJustPressed(KEYCODE_F4) || controller.WasButtonJustPressed(XBOX_BUTTON_DPAD_DOWN))
    {
        if (m_theGame)
        {
            m_theGame->MarkAllEntityAsDeadAndGarbage();
        }
    }

    if (!m_theGame->IsAttractMode())
    {
        if (g_theInput->WasKeyJustPressed(KEYCODE_F8))
        {
            DeleteAndCreateNewGame();
            m_theGame->SetAttractMode(true);
            m_theGame->SetPlayerShipIsReadyToSpawnBullet(!false);
        }
    }

    if (m_theGame->IsPlayerNameInputMode() &&
        g_theInput->WasKeyJustPressed(KEYCODE_ENTER))
    {
        m_isPaused = false;
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
void App::AdjustForPauseAndTimeDistortion(float& deltaSeconds) const
{
    if (!m_theGame->IsAttractMode())
    {
        if (m_isPaused)
            deltaSeconds = 0.f;

        if (m_isSlowMo)
            deltaSeconds *= 1 / 10.f;
    }
}

//----------------------------------------------------------------------------------------------------
void App::DeleteAndCreateNewGame()
{
    delete m_theGame;
    m_theGame = nullptr;

    m_theGame = new Game();
}
