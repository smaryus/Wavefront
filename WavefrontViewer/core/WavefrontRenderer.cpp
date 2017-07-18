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
#include <iostream>

using namespace std;

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

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

WavefrontRenderer::WavefrontRenderer(WavefrontFileReader& reader,
                                     const bool splitInTriangles/*=true*/)
: m_vboId(0)
, m_iboId(0)
, m_maxCoordinateValue(0.0f)
{
    const auto& object = reader.object();

    std::vector<Vertex> vbo;
    std::vector<uint32_t> ibo;

    vbo.reserve(object.vertices.size() + object.normals.size() +
                object.texCoords.size());

    std::unordered_map<KeyType, uint32_t, KeyTypeHash> duplicateVertices;

    m_maxCoordinateValue = 0;

    for( const auto& mesh : object.meshes )
    {
        Command command;
        command.index = (uint32_t)ibo.size();
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
                           (index.vertexIndex <= object.vertices.size()) );

                    vertex.position = object.vertices[index.vertexIndex-1];

                    m_maxCoordinateValue = std::max(m_maxCoordinateValue,
                                                    fabs(vertex.position.x));
                    m_maxCoordinateValue = std::max(m_maxCoordinateValue,
                                                    fabs(vertex.position.y));
                    m_maxCoordinateValue = std::max(m_maxCoordinateValue,
                                                    fabs(vertex.position.z));

                    if( index.normalIndex > 0 )
                    {
                        assert( (index.normalIndex > 0) &&
                               (index.normalIndex <= object.normals.size()) );

                        vertex.normal = object.normals[index.normalIndex-1];
                    }

                    if( index.textureIndex > 0 )
                    {
                        assert( (index.textureIndex > 0) &&
                               (index.textureIndex <= object.texCoords.size()) );

                        vertex.texture = object.texCoords[index.textureIndex-1];
                    }

                    uint32_t position = (uint32_t)vbo.size();
                    vbo.push_back(vertex);

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

        m_commands.push_back(command);
    }

    
    assert( !ibo.empty() && !vbo.empty() );

    glGenBuffers(1, &m_vboId);
    assert( m_vboId > 0 );
    glBindBuffer(GL_ARRAY_BUFFER, m_vboId);

    glBufferData(GL_ARRAY_BUFFER, vbo.size() * sizeof(vbo.front()),
                 vbo.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &m_iboId);
    assert( m_iboId > 0 );
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboId);


    glBufferData(GL_ELEMENT_ARRAY_BUFFER, ibo.size() * sizeof(ibo.front()),
                 ibo.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0); // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex), BUFFER_OFFSET(0));

    glEnableVertexAttribArray(1); // normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex), BUFFER_OFFSET(12));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

WavefrontRenderer::~WavefrontRenderer()
{
    if( m_vboId > 0 )
    {
        glDeleteBuffers(1, &m_vboId);
        m_vboId = 0;
    }

    if( m_iboId > 0 )
    {
        glDeleteBuffers(1, &m_iboId);
        m_iboId = 0;
    }
}

void WavefrontRenderer::draw() const
{
    if( m_iboId <= 0 )
    {
        assert( false );
        return;
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboId);

    for( auto& command : m_commands )
    {
        GLenum drawType = (command.type == Command::Triangles)
                ? GL_TRIANGLES : GL_TRIANGLE_FAN;

        glDrawElements(drawType, command.count, GL_UNSIGNED_INT,
                       (void*)(command.index * sizeof(uint32_t)));
    }

    glBindVertexArrayOES(0);
}
