#include "EditorLayer.h"
#include "Core/Bootstrapper.h"
#include "Core/Logger/Log.h"

int main(int argc, char** argv)
{
    Engine::Log::Initialize();

    const auto app = new Engine::Bootstrapper;
    app->Run();

    delete app;
}