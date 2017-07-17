//
//  types.h
//  WavefrontViewer
//
//  Created by Marius Sincovici on 17/07/2017.
//  Copyright © 2017 Marius Sincovici. All rights reserved.
//

#ifndef types_h
#define types_h

/**
 * Represents a 3D floating point
 */
template<class T>
struct Vec3
{
    T x; /// x-coordinate
    T y; /// y-coordinate
    T z; /// z-coordinate

    /**
     * Default constructor
     */
    Vec3(): x(T(0)), y(T(0)), z(T(0)) {}
};

typedef Vec3<float> fvec3;
typedef Vec3<int> ivec3;

struct IndexData
{
    int vertexIndex;
    int normalIndex;
    int textureIndex;

    IndexData() : vertexIndex(0), normalIndex(0), textureIndex(0) {}

    bool operator== (const IndexData& other) const
    {
        return ((this->vertexIndex == other.vertexIndex) &&
                (this->normalIndex == other.normalIndex) &&
                (this->textureIndex == other.textureIndex));
    }

};

struct Vertex
{
    fvec3 position;
    fvec3 normal;
    fvec3 texture;

    Vertex() : position(), normal(), texture() {}
};

#endif /* types_h */
