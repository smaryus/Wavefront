//
//  WavefrontFileReader.cpp
//  WavefrontViewer
//
//  Created by Marius Sincovici on 17/07/2017.
//  Copyright © 2017 Marius Sincovici. All rights reserved.
//

#include "WavefrontFileReader.h"

#include <fstream>
#include <cassert>
#include <vector>
#include <iostream>
#include <numeric>
#include <stdexcept>

#include "WavefrontObject.hpp"

using namespace std;
namespace WavefrontFileReader
{
#pragma mark - Private definition
    
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
                  const std::string& delimiters = "\t #");
    
    /**
     * Process a face ('f ...') from Wavefront file.
     *
     * @param tokens - list of tokens split using delimitates from @see tokenize
     *              method. The first token is always 'f'.
     *
     * @return A face object
     */
    Face processFace(const std::vector<std::string>& tokens);
    
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
    fvec3 processVec3(const std::vector<std::string>& tokens);
    
    
    std::shared_ptr<IObject> loadFile(const string& filePath)
    {
        ifstream file(filePath);
        
        if( !file.is_open() )
        {
            throw std::runtime_error("Could not open file");
        }
        
        return loadFile(file);
    }
    
    std::shared_ptr<IObject> loadFile(std::istream& stream)
    {
        string line;
        std::vector<std::string> tokens;
        
        std::shared_ptr<IObject> objPtr = std::shared_ptr<IObject>(new Object());
        auto& object = *(Object*)(objPtr.get());
        
        while( stream.good() )
        {
            stream >> std::ws;
            
            if( ! std::getline(stream, line) )
            {
                break;
            }
            
            if( !line.empty() )
            {
                if( line.back() == '\r' )
                {
                    line.pop_back();
                }
            }
            
            if( line.empty() )
            {
                continue;
            }
            
            assert(line.front() != ' ');
            
            if( line.front() == '#' )
            {
                // ignore white spaces
                continue;
            }
            
            tokenize(line, tokens);
            if( tokens.empty() )
            {
                continue;
            }
            
            const std::string& type = tokens[0];
            
            if( type == "v" )
            {
                // vertex
                auto vertex = processVec3(tokens);
                object.vertices.push_back(vertex);
            }
            else if( type == "vt")
            {
                // texture coordinates
                auto coord = processVec3(tokens);
                object.texCoords.push_back(coord);
            }
            else if( type == "vn")
            {
                // normal
                auto normal = processVec3(tokens);
                object.normals.push_back(normal);
            }
            else if( type == "g" )
            {
                // group name
                Mesh g;
                g.name.reserve(line.size());
                for( auto it = tokens.begin()+1; it != tokens.end(); ++it )
                {
                    if( !g.name.empty() )
                    {
                        g.name += " ";
                    }
                    g.name += *it;
                }
                
                object.meshes.push_back(std::move(g));
            }
            else if( type == "f" )
            {
                // face
                if( object.meshes.empty() )
                {
                    object.meshes.push_back(Mesh());
                }
                
                auto& mesh = object.meshes.back();
                
                auto face = processFace(tokens);
                mesh.numberOfElementsInFace = int(face.indices.size());
                mesh.faces.push_back(std::move(face));
            }
        }
        
        return objPtr;
    }
    
    bool validateObject(const Object& object)
    {
        for( const auto& mesh : object.meshes )
        {
            for( auto& face : mesh.faces )
            {
                for( auto& index : face.indices )
                {
                    if( abs(index.vertexIndex) > object.vertices.size() )
                    {
                        return false;
                    }
                    
                    if( abs(index.textureIndex) > object.texCoords.size() )
                    {
                        return false;
                    }
                    
                    if( abs(index.normalIndex) > object.normals.size() )
                    {
                        return false;
                    }
                }
            }
        }
        return true;
    }
    
#pragma mark - Private methods
    void tokenize(const std::string& str,
                  vector<string>& tokens,
                  const std::string& delimiters)
    {
        tokens.clear();
        
        if( str.empty() )
        {
            return;
        }
        
        size_t startPos = 0;
        size_t endPos = str.find_first_of(delimiters);
        while (endPos != std::string::npos )
        {
            if( startPos < endPos )
            {
                auto token = str.substr(startPos, endPos-startPos);
                if( token.front() == '#' )
                {
                    // comment detected, ignore everything till the end of line
                    return;
                }
                tokens.push_back(std::move(token));
            }
            
            startPos = endPos + 1;
            endPos = str.find_first_of(delimiters, startPos);
        }
        
        auto token = str.substr(startPos);
        tokens.push_back(std::move(token));
    }
    
    Face processFace(const vector<string>& tokens)
    {
        Face face;
        
        for( auto it = tokens.begin()+1; it != tokens.end(); ++it )
        {
            IndexData indexData;
            
            const char* ptr = (*it).c_str();
            char* pEnd = nullptr;
            
            indexData.vertexIndex = std::strtod(ptr, &pEnd);
            
            if( indexData.vertexIndex <= 0 )
            {
                continue;
            }
            
            if( *pEnd == '/' )
            {
                indexData.textureIndex = std::strtod(pEnd+1, &pEnd);
                
                if( *pEnd == '/' )
                {
                    indexData.normalIndex = std::strtod(pEnd+1, &pEnd);
                    assert(indexData.normalIndex > 0);
                }
            }
            
            face.indices.push_back(indexData);
        }
        
        return face;
    }
    
    fvec3 processVec3(const vector<string>& tokens)
    {
        fvec3 v;
        
        size_t tokensSize = tokens.size();
        
        size_t i = 1;
        v.x = (i < tokensSize) ? std::stof(tokens[i++].c_str()) : 0.0f;
        v.y = (i < tokensSize) ? std::stof(tokens[i++].c_str()) : 0.0f;
        v.z = (i < tokensSize) ? std::stof(tokens[i++].c_str()) : 0.0f;
        
        return v;
    }
}
