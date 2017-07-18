//
//  types.h
//  WavefrontViewer
//
//  Created by Marius Sincovici on 17/07/2017.
//  Copyright © 2017 Marius Sincovici. All rights reserved.
//

#ifndef types_h
#define types_h

#include <cinttypes>

/**
 * Represents a 3D floating point
 */
template<class T>
struct Vec3
{
    T x = T(0); /// x-coordinate
    T y = T(0); /// y-coordinate
    T z = T(0); /// z-coordinate

    bool operator== (const Vec3& other) const
    {
        return ((this->x == other.x) &&
                (this->y == other.y) &&
                (this->z == other.z));
    }
};

typedef Vec3<float> fvec3;
typedef Vec3<int> ivec3;

/**
 * Store face indices read from file
 */
struct IndexData
{
    int vertexIndex = 0; /// vertex index
    int normalIndex = 0; /// normal index
    int textureIndex = 0; /// texture index

    /**
     * Compare if two objects are equal
     * @param other - object to which this is compared
     * @return true when the objects are equal
     */
    bool operator== (const IndexData& other) const
    {
        return ((this->vertexIndex == other.vertexIndex) &&
                (this->normalIndex == other.normalIndex) &&
                (this->textureIndex == other.textureIndex));
    }

};

/**
 * Vertex structure used by VBO to render in opengl.
  */
struct Vertex
{
    fvec3 position; /// pozition
    fvec3 normal; /// normal
    fvec3 texture; /// texture coordinate

    bool operator== (const Vertex& other) const
    {
        return ((this->position == other.position) &&
                (this->normal == other.normal) &&
                (this->texture == other.texture));
    }

};

/**
 * Render command
 */
struct Command
{
    /// How the object should be render. As a triangle or quad
    enum Type
    {
        Triangles,
        Quads
    };

    Type type = Triangles; /// render type
    uint32_t index = 0; /// starting index from current VBO
    uint32_t count = 0; /// number of elements that need to be drawn
};

#endif /* types_h */
