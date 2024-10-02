#pragma once

#include "Mesh.h"

namespace RenderingEngine 
{ 
    class MeshImporter
    {
    public:
        static const Ref<Mesh> CreateMesh(const char* path);
        static const Ref<Mesh> CreatePlane(const float size = 1);
        static const Ref<Mesh> CreateCube(const float size = 1);
        static const Ref<Mesh> CreateSphere(const float radius = 1);
    };    
}