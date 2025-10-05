#pragma once

#include "Mesh.h"

namespace Engine 
{ 
    class MeshImporter
    {
    public:
        static const Ptr<Mesh>& CreateMesh(const std::string& path);
        
        static const Ptr<Mesh>& CreatePlane();
        static const Ptr<Mesh>& CreateCube();
        static const Ptr<Mesh>& CreateSphere();
    };    
}