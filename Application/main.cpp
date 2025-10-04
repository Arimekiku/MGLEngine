#include "Core/Logger/Log.h"
#include "Core/Bootstrapper.h"
#include "Core/Layer/ImGui/GuiLayer.h"
#include "EditorLayer.h"

int main(int argc, char** argv)
{
    Engine::Log::Initialize();

    const auto app = new Engine::Bootstrapper;

    app->AddLayer(std::make_shared<Engine::GuiLayer>());
    app->AddLayer(std::make_shared<Engine::EditorLayer>());
    app->Run();

    delete app;
}