#pragma once

#include "Core/Window.h"
#include "Core/Layer/Layer.h"

namespace Engine
{
    class GuiLayer final : public Layer
    {
    public:
        GuiLayer(const Ref<Window>& window);
        ~GuiLayer() override;

    	void Begin() const;
    	void End() const;

    private:
        void LoadLayout();
		void SaveLayout();

        Ref<Window> m_Parent = nullptr;

        float m_Time = 1 / 60.f;
    };
}
