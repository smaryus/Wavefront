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

using namespace std;

WavefrontFileReader::WavefrontFileReader(const string& filePath)
: m_filePath(filePath)
, m_object()
{
    loadFile(filePath);
}

void WavefrontFileReader::loadFile(const std::string& filePath)
{
    ifstream file(filePath);

    if( !file.is_open() )
    {
        return;
    }

    string line;
    std::vector<std::string> tokens;

    while( file.good() )
    {
        file >> std::ws;

        if( ! std::getline(file, line) )
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

        this->tokenize(line, tokens);
        if( tokens.empty() )
        {
            continue;
        }

        const std::string& type = tokens[0];

        if( type == "v" )
        {
            // vertex
            vec3 vertex = processVec3(tokens);
            m_object.vertices.push_back(vertex);
        }
        else if( type == "vt")
        {
            // texture coordinates
            vec3 coord = processVec3(tokens);
            m_object.texCoords.push_back(coord);
        }
        else if( type == "vn")
        {
            // normal
            vec3 normal = processVec3(tokens);
            m_object.normals.push_back(normal);
        }
        else if( type == "g" )
        {
            // group name
            Mesh g;
            g.name.reserve(line.size());
            for( auto it = tokens.begin()+1; it != tokens.end(); ++it )
            {
                g.name = *it + " ";
            }

            m_object.meshes.push_back(std::move(g));
        }
        else if( type == "f" )
        {
            // face
            if( m_object.meshes.empty() )
            {
                m_object.meshes.push_back(Mesh());
            }

            auto& mesh = m_object.meshes.back();

            auto face = processFace(tokens);

            mesh.faces.push_back(std::move(face));
        }
    }

}

void WavefrontFileReader::tokenize(const std::string& str,
                                   vector<string>& tokens,
                                   const std::string& delimiters) const
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

WavefrontFileReader::Face
WavefrontFileReader::processFace(const vector<string>& tokens) const
{
    Face face;

    for( auto it = tokens.begin()+1; it != tokens.end(); ++it )
    {
        const char* ptr = (*it).c_str();
        char* pEnd = nullptr;

        int vertexIndex = std::strtod(ptr, &pEnd);

        if( vertexIndex <= 0 )
        {
            continue;
        }

        face.vertexIndices.push_back(vertexIndex-1);

        if( *pEnd == '/' )
        {
            int textureIndex = std::strtod(pEnd+1, &pEnd);
            if(textureIndex > 0)
            {
                face.textureIndices.push_back(textureIndex-1);
            }

            if( *pEnd == '/' )
            {
                int normalIndex = std::strtod(pEnd+1, &pEnd);
                assert(normalIndex > 0);

                face.normalIndices.push_back(normalIndex-1);
            }
        }
    }

    return face;
}

vec3 WavefrontFileReader::processVec3(const vector<string>& tokens) const
{
    vec3 v;

    size_t tokensSize = tokens.size();

    size_t i = 1;
    v.x = (i < tokensSize) ? std::stof(tokens[i++].c_str()) : 0.0f;
    v.y = (i < tokensSize) ? std::stof(tokens[i++].c_str()) : 0.0f;
    v.z = (i < tokensSize) ? std::stof(tokens[i++].c_str()) : 0.0f;
    
    return v;
}
