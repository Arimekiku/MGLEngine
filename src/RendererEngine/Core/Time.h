#pragma once

namespace RenderingEngine
{
    class Time
    {
    public:
        explicit Time(const float time) : m_Time(time) { }

        float GetSeconds() const { return m_Time; }
        float GetMilliseconds() const { return m_Time * 1000; }
    private:
        float m_Time = 0;
    };
}
