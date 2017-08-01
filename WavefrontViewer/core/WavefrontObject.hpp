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
    struct Face
    {
        std::vector<IndexData> indices;
    };
    
    struct Mesh
    {
        std::string name; /// Mesh name if exist in file
        std::vector<Face> faces; /// List with all the faces that describe the mesh
        int numberOfElementsInFace = 0; /// number of index groups in a face
    };
    
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
        /// List with all the positions from file
        std::vector<fvec3> vertices;
        
        /// List with all the texture coordinates from file
        std::vector<fvec3> texCoords;
        
        /// List with all the normals from file
        std::vector<fvec3> normals;
        
        /// List with all the meshes from file
        std::vector<Mesh> meshes;
        
        mutable VertexBuffer m_vertexBuffer;
    };
}

#endif /* WavefrontObject_hpp */
