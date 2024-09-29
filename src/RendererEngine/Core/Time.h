#pragma once

namespace RenderingEngine
{
    class Time
    {
    public:
        explicit Time(const float time)
            : m_Time(time) { }

        [[nodiscard]] float GetSeconds() const { return m_Time; }
        [[nodiscard]] float GetMilliseconds() const { return m_Time * 1000; }
    private:
        float m_Time = 0;
    };
}
