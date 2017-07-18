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

#ifndef NO_OPENGL
#include <OpenGLES/ES2/glext.h>
#endif

#include "types.h"

// forward declaration
class WavefrontFileReader;

/**
 * Renders an wavefront file
 */
class WavefrontRenderer
{
public:

#ifdef NO_OPENGL
    WavefrontRenderer() {};
#endif

    /**
     * Existing wavefront reader file
     * @param reader - this needs to have all the information read
     * @param splitInTriangles - specify if it should transform everithing in
     *                  triangles if a face has more then 3 indices.
     *                  By default is true
     */
    WavefrontRenderer(const WavefrontFileReader& reader,
                      const bool splitInTriangles = true);

    ~WavefrontRenderer();

    /**
     * Called to render the object
     */
    void draw() const;

public:
    float maxCoordinateValue() const { return m_maxCoordinateValue; }

protected:
    void generateBuffers(const WavefrontFileReader& reader,
                         const bool splitInTriangles,
                         std::vector<Vertex>& vbo,
                         std::vector<uint32_t>& ibo);

private:
    void generateOpenGLBuffers(std::vector<Vertex>& vbo,
                               std::vector<uint32_t>& ibo);

private:
#ifndef NO_OPENGL
    GLuint m_vboId = 0; /// opengl vertex buffer object id
    GLuint m_iboId = 0; /// opengl index buffer object id
#endif
    std::vector<Command> m_commands; /// list with commands to draw the object
    float m_maxCoordinateValue = 0.0f; /// store the maximum coordinate value to calculate scale factor
};

#endif /* WavefrontFileRenderer_h */
