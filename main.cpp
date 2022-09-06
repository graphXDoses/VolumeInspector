#include "ngnpch.h"
#include "VolumeEngine.h"

#ifdef DEBUG
int main(int argc, char** argv)
{
    VolumeEngine::Log::Init();
    NGN_CORE_INFO("DEBUG MODE");
    auto app = VolumeEngine::CreateApplication("Ogotomos - Demo");
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
    VolumeEngine::Log::Init();
    NGN_CORE_INFO("RELEASE MODE");
    auto app = VolumeEngine::CreateApplication("Ogotomos - Demo");
    NGN_TRACE("Application Created!");

    NGN_CORE_INFO("Application Starting..");
    app->Run();

    delete app;
    NGN_WARN("Application Closed");

    return 0;
}
#endif
