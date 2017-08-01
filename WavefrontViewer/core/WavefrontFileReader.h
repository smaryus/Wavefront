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

#include "IObject.h"


/**
 * Load and parse a Wavefront .obj file
 * https://en.wikipedia.org/wiki/Wavefront_.obj_file
 */
namespace WavefrontFileReader
{
    /**
     * Load and parse the specified Wavefront file
     * @param stream - file stream
     */
    std::shared_ptr<IObject> loadFile(std::istream& stream);
    
    /**
     * Class constructor
     * @param filePath - full path to the Wavefront file
     */
    std::shared_ptr<IObject> loadFile(const std::string& filePath);

    /**
     * Validates vertex, normal, texture indices
     * @return Return true if the indices values are in range, false otherwise
     */
    bool validateObject(const IObject& object);
};

#endif /* WavefrontFileReader_h */
