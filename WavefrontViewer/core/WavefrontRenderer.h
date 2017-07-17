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

#include "types.h"

class WavefrontFileReader;

class WavefrontRenderer
{
public:
    WavefrontRenderer(WavefrontFileReader& reader);

private:
    std::vector<Vertex> m_vbo;
    std::vector<unsigned int> m_ibo;
    int m_normalsOffset, m_texCoordOffset;
};

#endif /* WavefrontFileRenderer_h */
