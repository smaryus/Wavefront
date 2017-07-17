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
    };

public:
    /**
     * Class constructor
     * @param filePath - full path to the Wavefront file
     */
    WavefrontFileReader(const std::string& filePath);

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
     * @param filePath - full path to the file
     */
    void loadFile(const std::string& filePath);

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
