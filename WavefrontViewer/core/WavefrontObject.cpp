//
//  Wavefrontcpp
//  WavefrontViewer
//
//  Created by candidate on 01/08/2017.
//  Copyright © 2017 Marius Sincovici. All rights reserved.
//

#include "WavefrontObject.hpp"

#include <unordered_map>
#include <algorithm>
#include <cassert>
#include <iostream>

#include "types.h"

namespace WavefrontFileReader
{
    typedef IndexData KeyType;

    /**
     * Used for hashing a IndexData. Needed for unordered_map
     */
    struct KeyTypeHash
    {
        std::size_t operator()(const KeyType& k) const
        {
            return k.vertexIndex ^ k.normalIndex ^ k.textureIndex;
        }
    };
    
    void Object::generateVertexBuffers(const bool splitInTriangles) const
    {
        m_vertexBuffer.clear();
        
        std::unordered_map<KeyType, uint32_t, KeyTypeHash> duplicateVertices;
        
        double maxCoordinateValue = 0;
        
        for( const auto& mesh : meshes )
        {
            auto& ibo = m_vertexBuffer.ibo;
            
            Command command;
            command.index = (uint32_t)m_vertexBuffer.ibo.size();
            command.type = (splitInTriangles || (mesh.numberOfElementsInFace == 3) )
            ? Command::Triangles : Command::Quads;
            
            for( const auto& face: mesh.faces )
            {
                short i = 0;
                for( auto& index : face.indices )
                {
                    if( splitInTriangles && (++i >= 4) )
                    {
                        // make triangles from quads 1 2 3 4 --> (1 2 3) (1 3 4)
                        auto a = *(ibo.end() - 3);
                        auto b = *(ibo.end() - 1);
                        
                        ibo.push_back(a);
                        ibo.push_back(b);
                    }
                    
                    const auto it = duplicateVertices.find(index);
                    
                    if( it == duplicateVertices.end() )
                    {
                        // not found. Add it to vbo, ibo and save the index to map
                        Vertex vertex;
                        
                        assert( (index.vertexIndex > 0) &&
                               (index.vertexIndex <= vertices.size()) );
                        
                        vertex.position = vertices[index.vertexIndex-1];
                        
                        maxCoordinateValue = std::max(maxCoordinateValue,
                                                        fabs(vertex.position.x));
                        maxCoordinateValue = std::max(maxCoordinateValue,
                                                        fabs(vertex.position.y));
                        maxCoordinateValue = std::max(maxCoordinateValue,
                                                        fabs(vertex.position.z));
                        
                        if( index.normalIndex > 0 )
                        {
                            assert( (index.normalIndex > 0) &&
                                   (index.normalIndex <= normals.size()) );
                            
                            vertex.normal = normals[index.normalIndex-1];
                        }
                        
                        if( index.textureIndex > 0 )
                        {
                            assert( (index.textureIndex > 0) &&
                                   (index.textureIndex <= texCoords.size()) );
                            
                            vertex.texture = texCoords[index.textureIndex-1];
                        }
                        
                        uint32_t position = (uint32_t)m_vertexBuffer.vbo.size();
                        m_vertexBuffer.vbo.push_back(vertex);
                        
                        duplicateVertices[index] = position;
                        
                        ibo.push_back( position );
                    }
                    else
                    {
                        // already exist, only add it to index buffer
                        assert( it->first == index );
                        
                        auto position = it->second;
                        ibo.push_back( position );
                    }
                }
            }
            
            command.count = (int)ibo.size() - command.index;
            
            m_vertexBuffer.commands.push_back(command);
            m_vertexBuffer.scale = maxCoordinateValue;
        }
    }
}
