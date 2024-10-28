#include "RendererEngine/Core/Logger/Log.h"
#include "RendererEngine/Core/Bootstrapper.h"
#include "RendererEngine/Core/Layer/ImGui/GuiLayer.h"
#include "Editor/EditorLayer.h"

int main(int argc, char** argv)
{
    RenderingEngine::Log::Initialize();

    const auto app = new RenderingEngine::Bootstrapper;

    app->AddLayer(new RenderingEngine::GuiLayer);
    app->AddLayer(new RenderingEngine::EditorLayer);
    app->Run();

    delete app;
}
