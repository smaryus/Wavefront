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
class WavefrontFileReader
{
public:
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

public:
    /**
     * Class constructor
     * @param filePath - full path to the Wavefront file
     */
    WavefrontFileReader(const std::string& filePath);

    /**
     * Class constructor with a stream object
     * @param stream - parse data from an existing stream
     */
    WavefrontFileReader(std::istream& stream);

    /**
     * Validates vertex, normal, texture indices
     * @return Return true if the indices values are in range, false otherwise
     */
    bool validateObject() const;

public:
    /**
     * Wavefront file representation
     *
     * @return Returns an @see Object structure.
     */
    const Object& object() const { return m_object; }

private:
    /**
     * Load and parse the specified Wavefront file
     * @param stream - file stream
     */
    void loadFile(std::istream& stream);

    /**
     * Tokenize string using the specified delimiters.
     *
     * @param str - string that will be tokenized
     * @param tokens - vector with tokens. Vector will be cleared before adding
     *                  new tokens
     * @param delimiters - delimiters used to tokenize the string
     */
    void tokenize(const std::string& str,
                  std::vector<std::string>& tokens,
                  const std::string& delimiters = "\t #") const;

    /**
     * Process a face ('f ...') from Wavefront file.
     *
     * @param tokens - list of tokens split using delimitates from @see tokenize
     *              method. The first token is always 'f'.
     *
     * @return A face object
     */
    Face processFace(const std::vector<std::string>& tokens) const;

    /**
     * Fill a @see vec3 object with the information from tokens.
     * Used to read vertex position, texture coords and normals.
     *
     * @param tokens - list of tokens split using delimitates from @see tokenize
     *              method. The first token is a string representing the type
     *              of the coordinates ('v', 'vt', 'vn' ...).
     *
     * @return Returns a vec3 object. If there are not sufficient tokens for
     *          all vec3 components they are set to zero
     */
    fvec3 processVec3(const std::vector<std::string>& tokens) const;

private:
    /// Full file path pointing to the loaded obj file
    std::string m_filePath;

    /// Wavefront file representation
    Object  m_object;
};

#endif /* WavefrontFileReader_h */
