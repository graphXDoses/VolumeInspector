#pragma once
#include "Engine/Core/Base.h"
#include "Engine/Core/Application.h"

extern MobiusEngine::Application* MobiusEngine::CreateApplication();

#ifdef MBS_DEBUG
int main(int argc, char** argv)
{
    MobiusEngine::Log::Init();
    NGN_CORE_INFO("DEBUG MODE");
    auto app = MobiusEngine::CreateApplication();
    NGN_TRACE("Application Created!");

    NGN_CORE_INFO("Application Starting..");
    app->Run();

    delete app;
    NGN_WARN("Application Closed");

    return 0;
}
#else
int WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nShowCmd
)
{
    MobiusEngine::Log::Init();
    NGN_CORE_INFO("RELEASE MODE");
    auto app = MobiusEngine::CreateApplication();
    NGN_TRACE("Application Created!");

    NGN_CORE_INFO("Application Starting..");
    app->Run();

    delete app;
    NGN_WARN("Application Closed");

    return 0;
}
#endif

