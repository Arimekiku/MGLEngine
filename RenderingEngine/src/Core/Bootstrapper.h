#pragma once

#include "Engine.h"

namespace RenderingEngine
{
	class Bootstrapper
	{
	public:
		Bootstrapper();
		virtual ~Bootstrapper();

		void Run();
	};
}