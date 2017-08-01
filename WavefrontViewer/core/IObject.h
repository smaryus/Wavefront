//
//  IObject..h
//  WavefrontViewer
//
//  Created by candidate on 01/08/2017.
//  Copyright © 2017 Marius Sincovici. All rights reserved.
//

#ifndef IObject__h
#define IObject__h

#include <memory>
#include <vector>
#include <cinttypes>
#include <string>

struct VertexBuffer
{
    std::vector<Vertex> vbo;
    std::vector<uint32_t> ibo;
    float scale = 1.0;
    std::vector<Command> commands;
    
    bool empty() const { return vbo.empty() && ibo.empty(); }
    
    void clear() { return vbo.clear(); ibo.clear(); }
};

struct Face
{
    std::vector<IndexData> indices;
};

struct Mesh
{
    std::string name; /// Mesh name if exist in file
    std::vector<Face> faces; /// List with all the faces that describe the mesh
    int numberOfElementsInFace = 0; /// number of index groups in a face
};

class IObject
{
public:
    virtual ~IObject() {}
    
    virtual const VertexBuffer& vertexBuffer() const = 0;
    virtual bool empty() const = 0;
    
    /// List with all the positions from file
    std::vector<fvec3> vertices;
    
    /// List with all the texture coordinates from file
    std::vector<fvec3> texCoords;
    
    /// List with all the normals from file
    std::vector<fvec3> normals;
    
    /// List with all the meshes from file
    std::vector<Mesh> meshes;
};


#endif /* IObject__h */
