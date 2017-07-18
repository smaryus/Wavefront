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

using namespace std;

WavefrontFileReader::WavefrontFileReader(const string& filePath)
: m_filePath(filePath)
, m_object()
{
    ifstream file(filePath);

    if( !file.is_open() )
    {
        throw std::runtime_error("Could not open file");

        return;
    }

    loadFile(file);
}

WavefrontFileReader::WavefrontFileReader(std::istream& stream)
{
    loadFile(stream);
}

bool WavefrontFileReader::validateObject() const
{
    for( const auto& mesh : m_object.meshes )
    {
        for( auto& face : mesh.faces )
        {
            for( auto& index : face.indices )
            {
                if( abs(index.vertexIndex) > m_object.vertices.size() )
                {
                    return false;
                }

                if( abs(index.textureIndex) > m_object.texCoords.size() )
                {
                    return false;
                }

                if( abs(index.normalIndex) > m_object.normals.size() )
                {
                    return false;
                }
            }
        }
    }
    return true;
}

void WavefrontFileReader::loadFile(std::istream& stream)
{
    string line;
    std::vector<std::string> tokens;

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

        this->tokenize(line, tokens);
        if( tokens.empty() )
        {
            continue;
        }

        const std::string& type = tokens[0];

        if( type == "v" )
        {
            // vertex
            auto vertex = processVec3(tokens);
            m_object.vertices.push_back(vertex);
        }
        else if( type == "vt")
        {
            // texture coordinates
            auto coord = processVec3(tokens);
            m_object.texCoords.push_back(coord);
        }
        else if( type == "vn")
        {
            // normal
            auto normal = processVec3(tokens);
            m_object.normals.push_back(normal);
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
            mesh.numberOfElementsInFace = int(face.indices.size());
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

fvec3 WavefrontFileReader::processVec3(const vector<string>& tokens) const
{
    fvec3 v;

    size_t tokensSize = tokens.size();

    size_t i = 1;
    v.x = (i < tokensSize) ? std::stof(tokens[i++].c_str()) : 0.0f;
    v.y = (i < tokensSize) ? std::stof(tokens[i++].c_str()) : 0.0f;
    v.z = (i < tokensSize) ? std::stof(tokens[i++].c_str()) : 0.0f;
    
    return v;
}
