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

struct VertexBuffer
{
    std::vector<Vertex> vbo;
    std::vector<uint32_t> ibo;
    float scale = 1.0;
    std::vector<Command> commands;
    
    bool empty() const { return vbo.empty() && ibo.empty(); }
    
    void clear() { return vbo.clear(); ibo.clear(); }
};

class IObject
{
public:
    virtual ~IObject() {}
    
    virtual const VertexBuffer& vertexBuffer() const = 0;
    virtual bool empty() const = 0;
};


#endif /* IObject__h */
