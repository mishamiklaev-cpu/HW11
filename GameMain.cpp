#include "Application.h"
#include "Logger.h"
#include "CrashHandler.h"
#include "ResourceSystem.h"
#include <iostream>
using namespace Roguelike;
int main()
{
    Engine::installCrashHandler();
    Engine::setupLogger();
    LOG_INFO("Application starting...");
    LOG_INFO("Crash handler registered for SIGSEGV, SIGABRT, SIGFPE, SIGILL, SIGINT");
    try
    {
        Application& app = Application::Instance();
        LOG_INFO("Application instance created");
        app.Run();
        LOG_INFO("Application finished normally");
    }
    catch (const Engine::ResourceLoadException& e)
    {
        LOG_ERROR(std::string("Resource load exception: ") + e.what());
    }
    catch (const std::runtime_error& e)
    {
        LOG_ERROR(std::string("Runtime error: ") + e.what());
    }
    catch (const std::exception& e)
    {
        LOG_ERROR(std::string("Exception: ") + e.what());
    }
    catch (...)
    {
        LOG_ERROR("Unknown exception occurred");
    }
    return 0;
}