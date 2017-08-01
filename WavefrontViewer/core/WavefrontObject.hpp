//
//  WavefrontObject.hpp
//  WavefrontViewer
//
//  Created by candidate on 01/08/2017.
//  Copyright © 2017 Marius Sincovici. All rights reserved.
//

#ifndef WavefrontObject_hpp
#define WavefrontObject_hpp

#include <vector>
#include <string>

#include "types.h"
#include "IObject.h"

namespace WavefrontFileReader
{
    /**
     * Store the indices for a face
     */
    
    
    /**
     * Represents the Wavefront file content
     */
    class Object final : public IObject
    {
    public:
        const VertexBuffer& vertexBuffer() const override
        {
            if( m_vertexBuffer.empty() )
            {
                generateVertexBuffers(true);
            }
            return m_vertexBuffer;
        }
        
        /**
         * Checks if all the elements from object are empty or not
         * @return Returns true if all the lists are empty and false otherwise
         */
        bool empty() const override
        { return vertices.empty() && texCoords.empty() && normals.empty() && meshes.empty(); }
        
        /**
         * Create opengl representations for buffers created with @see
         * generateBuffers
         */
        void generateVertexBuffers(const bool splitInTriangles) const;
        
    public:
        
        
        mutable VertexBuffer m_vertexBuffer;
    };
}

#endif /* WavefrontObject_hpp */
