//
//  WavefrontFileReaderTest.cpp
//  WavefrontViewer
//
//  Created by Marius Sincovici on 18/07/2017.
//  Copyright © 2017 Marius Sincovici. All rights reserved.
//

#include <gtest/gtest.h>

#include <sstream>

#include "WavefrontFileReader.h"

using namespace std;

// check if exteption is thrown when invalid path is given
TEST(WavefrontFileReader, WrongPath)
{
    bool readSuccessfull = true;
    try
    {
        WavefrontFileReader reader("");
    }
    catch (std::exception& ex)
    {
        readSuccessfull = false;
    }

    ASSERT_EQ(false, readSuccessfull);
}

// check if an empty stream is given to parse the result will be an empty object
TEST(WavefrontFileReader, EmptyStream)
{
    bool readSuccessfull = true;
    try
    {
        stringstream stream;
        WavefrontFileReader reader(stream);

        ASSERT_EQ(true, reader.object().empty());
    }
    catch (std::exception& ex)
    {
        readSuccessfull = false;
    }

    ASSERT_EQ(true, readSuccessfull);
}

// check if a stream with comments is given to parse the result will
// be an empty object
TEST(WavefrontFileReader, OnlyComments)
{
    bool readSuccessfull = true;
    try
    {
        stringstream stream;
        stream << "#line1 \n #line2 \n #line3 \n #line3 \n #line4 \n #line5";
        WavefrontFileReader reader(stream);

        ASSERT_EQ(true, reader.object().empty());
    }
    catch (std::exception& ex)
    {
        readSuccessfull = false;
    }

    ASSERT_EQ(true, readSuccessfull);
}

// check if a stream with positions section is given to parse
TEST(WavefrontFileReader, PositionsOnly)
{
    bool readSuccessfull = true;
    try
    {
        stringstream stream;
        stream << "v -0.500000 -0.500000 0.500000 \n\
        v 0.500000 -0.500000 0.500000 \n\
        v -0.500000 0.500000 0.500000 \n\
        # v 0.500000 0.500000 0.500000 \n\
        v -0.500000 0.500000 -0.500000 \n\
        v 0.500000 0.500000 -0.500000 \n \
        v -0.500000 -0.500000 -0.500000 \n \
        v 0.500000 -0.500000 -0.500000";

        WavefrontFileReader reader(stream);

        ASSERT_EQ(false, reader.object().empty());

        ASSERT_EQ(7, reader.object().vertices.size());
        ASSERT_EQ(true, reader.object().normals.empty());
        ASSERT_EQ(true, reader.object().texCoords.empty());
        ASSERT_EQ(true, reader.object().meshes.empty());
    }
    catch (std::exception& ex)
    {
        readSuccessfull = false;
    }

    ASSERT_EQ(true, readSuccessfull);
}

// check if a stream only with tex coords is given to parse
TEST(WavefrontFileReader, TextureCoordinatesOnly)
{
    bool readSuccessfull = true;
    try
    {
        stringstream stream;
        stream << R"OBJ(vt 0.000000 0.000000
        vt 1.000000 0.000000
        vt 0.000000 1.000000
        vt 1.000000 1.000000)OBJ";

        WavefrontFileReader reader(stream);

        ASSERT_EQ(false, reader.object().empty());

        ASSERT_EQ(true, reader.object().vertices.empty());
        ASSERT_EQ(true, reader.object().normals.empty());
        ASSERT_EQ(4, reader.object().texCoords.size());
        ASSERT_EQ(true, reader.object().meshes.empty());
    }
    catch (std::exception& ex)
    {
        readSuccessfull = false;
    }

    ASSERT_EQ(true, readSuccessfull);
}

// check if a stream with normals is given to parse the result
TEST(WavefrontFileReader, NormalsOnly)
{
    try
    {
        stringstream stream;
        stream << R"OBJ(vn 0.000000 0.000000 1.000000
        vn 0.000000 1.000000 0.000000
        vn 0.000000 0.000000 -1.000000
        vn 0.000000 -1.000000 0.000000
        vn 1.000000 0.000000 0.000000
        vn -1.000000 0.000000 0.000000)OBJ";

        WavefrontFileReader reader(stream);

        ASSERT_EQ(false, reader.object().empty());

        ASSERT_EQ(true, reader.object().vertices.empty());
        ASSERT_EQ(6, reader.object().normals.size());
        ASSERT_EQ(true, reader.object().texCoords.empty());
        ASSERT_EQ(true, reader.object().meshes.empty());
    }
    catch (std::exception& ex)
    {
        ASSERT_TRUE(false);
    }
}

// check if a stream with normals is given to parse the result
TEST(WavefrontFileReader, FaceOnly)
{
    try
    {
        stringstream stream;
        stream << R"OBJ(f 1/1/1 2/2/1 3/3/1
        f 3/3/1 2/2/1 4/4/1
        f 3/1/2 4/2/2 5/3/2
        f 5/3/2 4/2/2 6/4/2)OBJ";

        WavefrontFileReader reader(stream);

        ASSERT_EQ(false, reader.object().empty());

        ASSERT_EQ(true, reader.object().vertices.empty());
        ASSERT_EQ(true, reader.object().normals.empty());
        ASSERT_EQ(true, reader.object().texCoords.empty());
        ASSERT_EQ(1, reader.object().meshes.size());

        const auto& mesh = reader.object().meshes.front();
        ASSERT_EQ(true, mesh.name.empty());
        ASSERT_EQ(4, mesh.faces.size());
        ASSERT_EQ(3, mesh.numberOfElementsInFace);

        ASSERT_FALSE(reader.validateObject());
    }
    catch (std::exception& ex)
    {
        ASSERT_TRUE(false);
    }
}

// check Cube.obj file
TEST(WavefrontFileReader, Cube)
{
    try
    {
        WavefrontFileReader reader("cube.obj");

        ASSERT_EQ(false, reader.object().empty());

        ASSERT_EQ(8, reader.object().vertices.size());
        ASSERT_EQ(6, reader.object().normals.size());
        ASSERT_EQ(4, reader.object().texCoords.size());
        ASSERT_EQ(1, reader.object().meshes.size());

        const auto& mesh = reader.object().meshes.front();
        ASSERT_TRUE(mesh.name == "cube");
        ASSERT_EQ(12, mesh.faces.size());
        ASSERT_EQ(3, mesh.numberOfElementsInFace);

        ASSERT_TRUE(reader.validateObject());
    }
    catch (std::exception& ex)
    {
        ASSERT_TRUE(false);
    }
}

TEST(WavefrontFileReader, Ducky)
{
    try
    {
        WavefrontFileReader reader("ducky.obj");

        ASSERT_EQ(false, reader.object().empty());

        ASSERT_EQ(7895, reader.object().vertices.size());
        ASSERT_EQ(0, reader.object().normals.size());
        ASSERT_EQ(8850, reader.object().texCoords.size());
        ASSERT_EQ(4, reader.object().meshes.size());

        ASSERT_TRUE(reader.validateObject());

        auto& meshes = reader.object().meshes;

        int i = 0;
        ASSERT_TRUE(meshes[i].name == "Ducky Body");
        ASSERT_EQ(2816, meshes[i].faces.size());
        ASSERT_EQ(4, meshes[i].numberOfElementsInFace);

        ++i;

        ASSERT_TRUE(meshes[i].name == "Ducky Body Bill");
        ASSERT_EQ(2400, meshes[i].faces.size());
        ASSERT_EQ(4, meshes[i].numberOfElementsInFace);

        ++i;

        ASSERT_TRUE(meshes[i].name == "Eye Eye1");
        ASSERT_EQ(1512, meshes[i].faces.size());
        ASSERT_EQ(4, meshes[i].numberOfElementsInFace);

        ++i;

        ASSERT_TRUE(meshes[i].name == "Eye Eye1 Pupil");
        ASSERT_EQ(336, meshes[i].faces.size());
        ASSERT_EQ(4, meshes[i].numberOfElementsInFace);

    }
    catch (std::exception& ex)
    {
        ASSERT_TRUE(false);
    }
}

// check quad file
TEST(WavefrontFileReader, Quad)
{
    try
    {
        stringstream stream;
        stream << R"OBJ(v 0.000000 1.000000 1.000000
        v 0.000000 0.000000 1.000000
        v 1.000000 0.000000 1.000000
        v 1.000000 1.000000 1.000000
        v 0.000000 1.000000 0.000000
        v 0.000000 0.000000 0.000000
        v 1.000000 0.000000 0.000000
        v 1.000000 1.000000 0.000000
        f 1 2 3 4
        f 8 7 6 5
        f 4 3 7 8
        f 5 1 4 8
        f 5 6 2 1
        f 2 6 7 3)OBJ";

        WavefrontFileReader reader(stream);

        ASSERT_EQ(false, reader.object().empty());

        ASSERT_EQ(8, reader.object().vertices.size());
        ASSERT_EQ(true, reader.object().normals.empty());
        ASSERT_EQ(true, reader.object().texCoords.empty());
        ASSERT_EQ(1, reader.object().meshes.size());

        const auto& mesh = reader.object().meshes.front();
        ASSERT_EQ(6, mesh.faces.size());
        ASSERT_EQ(4, mesh.numberOfElementsInFace);

        ASSERT_TRUE(reader.validateObject());
    }
    catch (std::exception& ex)
    {
        ASSERT_TRUE(false);
    }
}

