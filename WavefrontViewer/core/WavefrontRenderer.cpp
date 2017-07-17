//
//  WavefrontRenderer.cpp
//  WavefrontViewer
//
//  Created by Marius Sincovici on 17/07/2017.
//  Copyright © 2017 Marius Sincovici. All rights reserved.
//

#include "WavefrontRenderer.h"

#include "WavefrontFileReader.h"

#include <algorithm>
#include <unordered_map>
#include <cassert>

using namespace std;

typedef IndexData KeyType;

struct KeyTypeHash
{
    std::size_t operator()(const KeyType& k) const
    {
        return k.vertexIndex ^ k.normalIndex ^ k.textureIndex;
    }
};

WavefrontRenderer::WavefrontRenderer(WavefrontFileReader& reader)
: m_vbo()
, m_ibo()
{
    const auto& object = reader.object();

    m_vbo.reserve(object.vertices.size() + object.normals.size() +
                  object.texCoords.size());


    std::unordered_map<KeyType, unsigned int, KeyTypeHash> duplicateVertices;

    for( const auto& mesh : object.meshes )
    {
        for( const auto& face: mesh.faces )
        {
            for( auto& index : face.indices )
            {
                const auto it = duplicateVertices.find(index);

                if( it == duplicateVertices.end() )
                {
                    // not found. Add it and save the index in vertex buffer
                    Vertex vertex;

                    assert( (index.vertexIndex > 0) &&
                           (index.vertexIndex <= object.vertices.size()) );

                    vertex.position = object.vertices[index.vertexIndex-1];

                    if( index.normalIndex > 0 )
                    {
                        vertex.normal = object.normals[index.normalIndex-1];
                    }

                    if( index.textureIndex > 0 )
                    {
                        vertex.texture = object.texCoords[index.textureIndex-1];
                    }

                    unsigned int position = (unsigned int)m_vbo.size();
                    m_vbo.push_back(vertex);

                    duplicateVertices[index] = position;

                    m_ibo.push_back( position );
                }
                else
                {
                    // already exist
                    auto position = it->second;
                    m_ibo.push_back( position );
                }
            }
        }
    }

//    std::copy(object.vertices.begin(), object.vertices.end(),
//              std::back_inserter(m_vbo));
//
//    m_normalsOffset = m_vbo.size();
//
//    std::copy(object.normals.begin(), object.normals.end(),
//              std::back_inserter(m_vbo));
//
//    m_texCoordOffset = m_vbo.size();
//
//    std::copy(object.texCoords.begin(), object.texCoords.end(),
//              std::back_inserter(m_vbo));
}

