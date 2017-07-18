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

// forward declaration
class WavefrontFileReader;

/**
 * Renders an wavefront file
 */
class WavefrontRenderer
{
public:
    /**
     * Existing wavefront reader file
     * @param reader - this needs to have all the information read
     * @param splitInTriangles - specify if it should transform everithing in
     *                  triangles if a face has more then 3 indices.
     *                  By default is true
     */
    WavefrontRenderer(WavefrontFileReader& reader,
                      const bool splitInTriangles = true);

    ~WavefrontRenderer();

    /**
     * Called to render the object
     */
    void draw() const;

public:
    float maxCoordinateValue() const { return m_maxCoordinateValue; }

private:
    GLuint m_vboId; /// opengl vertex buffer object id
    GLuint m_iboId; /// opengl index buffer object id
    std::vector<Command> m_commands; /// list with commands to draw the object
    float m_maxCoordinateValue; /// store the maximum coordinate value to calculate scale factor
};

#endif /* WavefrontFileRenderer_h */
