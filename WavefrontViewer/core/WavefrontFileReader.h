//
//  WavefrontFileReader.h
//  WavefrontViewer
//
//  Created by Marius Sincovici on 17/07/2017.
//  Copyright © 2017 Marius Sincovici. All rights reserved.
//

#ifndef WavefrontFileReader_h
#define WavefrontFileReader_h

#include <vector>
#include <string>
#include <cinttypes>

#include "types.h"

/**
 * Load and parse a Wavefront .obj file
 * https://en.wikipedia.org/wiki/Wavefront_.obj_file
 */
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
    struct Object
    {
        /// List with all the positions from file
        std::vector<fvec3> vertices;

        /// List with all the texture coordinates from file
        std::vector<fvec3> texCoords;

        /// List with all the normals from file
        std::vector<fvec3> normals;

        /// List with all the meshes from file
        std::vector<Mesh> meshes;

        /**
         * Checks if all the elements from object are empty or not
         * @return Returns true if all the lists are empty and false otherwise
         */
        bool empty() const { return vertices.empty() && texCoords.empty() &&
            normals.empty() && meshes.empty(); }
    };

    /**
     * Load and parse the specified Wavefront file
     * @param stream - file stream
     */
    Object loadFile(std::istream& stream);
    
    /**
     * Class constructor
     * @param filePath - full path to the Wavefront file
     */
    Object loadFile(const std::string& filePath);

    /**
     * Validates vertex, normal, texture indices
     * @return Return true if the indices values are in range, false otherwise
     */
    bool validateObject(const Object& object);
};

#endif /* WavefrontFileReader_h */
