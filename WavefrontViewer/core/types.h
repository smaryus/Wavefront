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
struct vec3
{
    float x; /// x-coordinate
    float y; /// y-coordinate
    float z; /// z-coordinate

    /**
     * Default constructor
     */
    vec3(): x(0), y(0), z(0) {}
};

#endif /* types_h */
