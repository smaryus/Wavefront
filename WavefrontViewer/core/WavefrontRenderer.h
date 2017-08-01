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
namespace WavefrontFileReader
{
    struct Object;
};

/**
 * Renders an wavefront file
 */
class WavefrontRenderer
{
public:

#ifdef NO_OPENGL
    // Default constructor for unit tests
    WavefrontRenderer() {};
#endif

    /**
     * Existing wavefront reader file
     * @param reader - this needs to have all the information read
     * @param splitInTriangles - specify if it should transform everithing in
     *                  triangles if a face has more then 3 indices.
     *                  By default is true
     */
    WavefrontRenderer(const WavefrontFileReader::Object& reader,
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
    float maxCoordinateValue() const { return m_maxCoordinateValue; }
    /**
     * Get the list of commands used to render the object
     * @return List of @see Command
     */
    const std::vector<Command>& commandsList() const { return m_commands; }

protected:
    /**
     * Generate buffers that later can be sent to opengl
     * @param reader - wavefront file reader
     * @param splitInTriangles - quads should be made from triangles
     * @param vbo - VBO buffer
     * @param ibo - index buffer object
     */
    void generateBuffers(const WavefrontFileReader::Object& reader,
                         const bool splitInTriangles,
                         std::vector<Vertex>& vbo,
                         std::vector<uint32_t>& ibo);

private:
    /**
     * Create opengl representations for buffers created with @see
     * generateBuffers
     * @param vbo - VBO buffer
     * @param ibo - index buffer object
     */
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
