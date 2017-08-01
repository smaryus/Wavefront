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

WavefrontRenderer::WavefrontRenderer(const IObject& object,
                                     const bool splitInTriangles/*=true*/)
{
    if( object.empty() )
    {
        assert(false);
        return;
    }

    m_vertexBuffer = object.vertexBuffer();
    assert(!m_vertexBuffer.empty());

    generateOpenGLBuffers();
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

    for( auto& command : m_vertexBuffer.commands )
    {
        GLenum drawType = (command.type == Command::Triangles)
                ? GL_TRIANGLES : GL_TRIANGLE_FAN;

        glDrawElements(drawType, command.count, GL_UNSIGNED_INT,
                       (void*)(command.index * sizeof(uint32_t)));
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void WavefrontRenderer::generateOpenGLBuffers()
{
    auto& ibo = m_vertexBuffer.ibo;
    auto& vbo = m_vertexBuffer.vbo;
    
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
