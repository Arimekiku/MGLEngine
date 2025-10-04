#include "Core/Logger/Log.h"
#include "Core/Bootstrapper.h"
#include "Core/Layer/ImGui/GuiLayer.h"
#include "EditorLayer.h"

int main(int argc, char** argv)
{
    Engine::Log::Initialize();

    const auto app = new Engine::Bootstrapper;

    app->AddLayer(new Engine::GuiLayer);
    app->AddLayer(new Engine::EditorLayer);
    app->Run();

    delete app;
}