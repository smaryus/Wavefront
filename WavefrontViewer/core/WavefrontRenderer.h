//
//  WavefrontRenderer.h
//  WavefrontViewer
//
//  Created by Marius Sincovici on 17/07/2017.
//  Copyright © 2017 Marius Sincovici. All rights reserved.
//

#ifndef WavefrontFileRenderer_h
#define WavefrontFileRenderer_h

#include <cstdio>
#include <vector>

#include <OpenGLES/ES2/glext.h>

#include "types.h"
#include "IObject.h"

/**
 * Renders an wavefront file
 */
class WavefrontRenderer
{
public:

    // Default constructor for unit tests
    WavefrontRenderer() {};

    /**
     * Existing wavefront reader file
     * @param reader - this needs to have all the information read
     * @param splitInTriangles - specify if it should transform everithing in
     *                  triangles if a face has more then 3 indices.
     *                  By default is true
     */
    WavefrontRenderer(const IObject& reader,
                      const bool splitInTriangles = true);

    /**
     * Class destructorgenerateBuffers
     */
    ~WavefrontRenderer();

    /**
     * Called to render the object
     */
    void draw() const;

public:
    float maxCoordinateValue() const { return m_vertexBuffer.scale; }

private:
    /**
     * Create opengl representations for buffers created with @see
     * generateBuffers
     * @param vbo - VBO buffer
     * @param ibo - index buffer object
     */
    void generateOpenGLBuffers();

private:
    GLuint m_vboId = 0; /// opengl vertex buffer object id
    GLuint m_iboId = 0; /// opengl index buffer object id

    VertexBuffer m_vertexBuffer;
};

#endif /* WavefrontFileRenderer_h */
