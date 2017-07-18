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
#include "WavefrontRenderer.h"

using namespace std;

class WavefrontRendererTest : public WavefrontRenderer
{
public:
    WavefrontRendererTest() : WavefrontRenderer() {}

    void process(const std::string& filePath, const bool splitInTriangles)
    {
        try
        {
            WavefrontFileReader reader(filePath);
            this->process(reader, splitInTriangles);
        }
        catch (std::exception& ex)
        {
            ASSERT_TRUE(false);
        }
    }

    void process(std::istream& stream, const bool splitInTriangles)
    {
        try
        {
            WavefrontFileReader reader(stream);
            this->process(reader, splitInTriangles);
        }
        catch (std::exception& ex)
        {
            ASSERT_TRUE(false);
        }
    }

    void process(const WavefrontFileReader reader, const bool splitInTriangles)
    {
        ASSERT_TRUE(m_vbo.empty());
        ASSERT_TRUE(m_ibo.empty());
        this->generateBuffers(reader, splitInTriangles,
                              m_vbo, m_ibo);
    }

    std::vector<Vertex> m_vbo;
    std::vector<uint32_t> m_ibo;
};

class RendererTest : public ::testing::Test
{
public:
    RendererTest()
    {}

    void SetUp(const std::string& fileName, bool triangulate)
    {
        m_rendrer.process(fileName, triangulate);
    }

    void SetUp(std::istream& stream, bool triangulate)
    {
        m_rendrer.process(stream, triangulate);
    }

    void TearDown()
    {
    }

    ~RendererTest()
    {
    }

    WavefrontRendererTest m_rendrer;
};

// Renderer doesn't crash when reader is empty
TEST_F(RendererTest, EmptyStream)
{
    stringstream stream;
    SetUp(stream, false);

    ASSERT_TRUE( m_rendrer.m_ibo.empty() );
    ASSERT_TRUE( m_rendrer.m_vbo.empty() );
}

TEST_F(RendererTest, Cube)
{
    SetUp("cube.obj", true);

    ASSERT_EQ(36, m_rendrer.m_ibo.size() );
    ASSERT_EQ(24, m_rendrer.m_vbo.size() );
}

TEST_F(RendererTest, Ducky)
{
    SetUp("ducky.obj", true);

    ASSERT_EQ(42384, m_rendrer.m_ibo.size() );
    ASSERT_EQ(8850, m_rendrer.m_vbo.size() );
}

// Same shape is saved as triangles and quads. Check if triagulation works
TEST(WavefrontRendererTest, SameFileDifferentRepresentation)
{
    WavefrontRendererTest render1;
    render1.process("humanoid_tri.obj", true);

    WavefrontRendererTest render2;
    render2.process("humanoid_quad.obj", true);

    ASSERT_TRUE( render1.m_vbo == render2.m_vbo );
    ASSERT_TRUE( render1.m_ibo == render2.m_ibo );

    // load without converting to triangles
    WavefrontRendererTest render3;
    render3.process("humanoid_quad.obj", false);

    // same VBO but different indexes
    ASSERT_TRUE( render3.m_vbo == render1.m_vbo );
    ASSERT_FALSE( render3.m_ibo == render1.m_ibo );
    ASSERT_TRUE( render3.m_vbo == render2.m_vbo );
    ASSERT_FALSE( render3.m_ibo == render2.m_ibo );

}
