#pragma once

#include "Mesh.h"

namespace RenderingEngine 
{ 
    class MeshImporter
    {
    public:
        static const Ref<Mesh>& CreateMesh(const std::string& path);
        
        static const Ref<Mesh>& CreatePlane();
        static const Ref<Mesh>& CreateCube();
        static const Ref<Mesh>& CreateSphere();
    };    
}