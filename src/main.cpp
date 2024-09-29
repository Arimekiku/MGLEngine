#define GLFW_INCLUDE_NONE
#define IMGUI_IMPL_OPENGL_LOADER_GLAD

#include "Renderer/Core/Logger/Log.h"
#include "Renderer/Core/Bootstrapper.h"
#include "Renderer/Core/Layer/ImGui/GuiLayer.h"
#include "Editor/Scene/SceneLayer.h"


int main(int argc, char** argv)
{
    RenderingEngine::Log::Initialize();

    const auto app = new RenderingEngine::Bootstrapper;

    app->AddLayer(new RenderingEngine::GuiLayer);
    app->AddLayer(new RenderingEngine::SceneLayer);
    app->Run();

    delete app;
}
