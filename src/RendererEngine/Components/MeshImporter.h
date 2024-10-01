#pragma once

#include "Mesh.h"

namespace RenderingEngine 
{ 
    class MeshImporter
    {
    public:
        static const Ref<Mesh> CreateMesh(const char* path);
    private:
    };    
}