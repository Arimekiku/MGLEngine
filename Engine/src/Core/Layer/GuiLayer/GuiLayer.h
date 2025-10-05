#pragma once

#include "Core/Window.h"
#include "Core/Layer/Layer.h"

namespace Engine
{
    class GuiLayer final : public Layer
    {
    public:
        GuiLayer(const Ptr<Window>& window);
        ~GuiLayer() override;

    	void Begin() const;
    	void End() const;

    private:
        void LoadLayout();
		void SaveLayout();

        Ptr<Window> m_Parent = nullptr;

        float m_Time = 1 / 60.f;
    };
}
