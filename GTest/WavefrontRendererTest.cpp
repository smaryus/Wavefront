//
//  WavefrontRendererTest.cpp
//  WavefrontViewer
//
//  Created by Marius Sincovici on 18/07/2017.
//  Copyright © 2017 Marius Sincovici. All rights reserved.
//

#include <stdio.h>

#include <gtest/gtest.h>

#include "WavefrontFileReader.h"

using namespace std;
using namespace WavefrontFileReader;

class RendererTest : public ::testing::Test
{
public:
    RendererTest()
    {}

    void SetUp(const std::string& fileName, bool triangulate)
    {
        m_object = WavefrontFileReader::loadFile(fileName)->vertexBuffer();
    }

    void SetUp(std::istream& stream, bool triangulate)
    {
        m_object = WavefrontFileReader::loadFile(stream)->vertexBuffer();
    }

    void TearDown()
    {
    }

    ~RendererTest()
    {
    }

    VertexBuffer m_object;
};

// Renderer doesn't crash when reader is empty
TEST_F(RendererTest, EmptyStream)
{
    stringstream stream;
    SetUp(stream, false);

    ASSERT_TRUE( m_object.empty() );
}

TEST_F(RendererTest, Cube)
{
    SetUp("cube.obj", true);

    ASSERT_EQ(36, m_object.ibo.size() );
    ASSERT_EQ(24, m_object.vbo.size() );
}

TEST_F(RendererTest, Ducky)
{
    SetUp("ducky.obj", true);

    ASSERT_EQ(42384, m_object.ibo.size() );
    ASSERT_EQ(8850, m_object.vbo.size() );
}

// Same shape is saved as triangles and quads. Check if triagulation works
//TEST(WavefrontRendererTest, SameFileDifferentRepresentation)
//{
//    auto render1 = WavefrontFileReader::loadFile("humanoid_tri.obj")->vertexBuffer();
//    auto render2 = WavefrontFileReader::loadFile("humanoid_quad.obj")->vertexBuffer();
//
//    ASSERT_FALSE(render1.vbo.empty());
//    ASSERT_FALSE(render1.ibo.empty());
//    ASSERT_TRUE( render1.vbo == render2.vbo );
//    ASSERT_TRUE( render1.ibo == render2.ibo );
//
//    ASSERT_EQ(render1.commands, render2.commands);
//
//    // load without converting to triangles
//    WavefrontRendererTest render3;
//    render3.process("humanoid_quad.obj", false);
//
//    // same VBO but different indexes
//    ASSERT_FALSE(render3.vbo.empty());
//    ASSERT_FALSE(render3.ibo.empty());
//
//    ASSERT_TRUE( render3.vbo == render1.vbo );
//    ASSERT_FALSE( render3.ibo == render1.ibo );
//    ASSERT_TRUE( render3.vbo == render2.vbo );
//    ASSERT_FALSE( render3.ibo == render2.ibo );
//
//    ASSERT_FALSE(render3.commandsList().empty());
//
//    ASSERT_NE(render1.commands, render3.commands);
//
//    ASSERT_EQ(Command::Triangles, render1.commandsList().front().type);
//    ASSERT_EQ(Command::Triangles, render2.commandsList().front().type);
//    ASSERT_EQ(Command::Quads, render3.commandsList().front().type);
//}
