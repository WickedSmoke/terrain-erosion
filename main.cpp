/****************************************************************************
    Copyright (C) 2012 Adrian Blumer (blumer.adrian@gmail.com)
    Copyright (C) 2012 Pascal Spörri (pascal.spoerri@gmail.com)
    Copyright (C) 2012 Sabina Schellenberg (sabina.schellenberg@gmail.com)

    All Rights Reserved.

    You may use, distribute and modify this code under the terms of the
    MIT license (http://opensource.org/licenses/MIT).
*****************************************************************************/

#include <iostream>
#include <stdlib.h>

#include "tclap/CmdLine.h"
#include "platform_includes.h"

#include "TerrainFluidSimulation.h"

using namespace std;

GLFWwindow* win = 0;
TerrainFluidSimulation* simulationPtr = 0;

void onWindowClose( GLFWwindow* )
{
    if (simulationPtr) simulationPtr->Stop();
}

void keyHandler( GLFWwindow*, int key, int /*code*/, int action, int mods )
{
    if( action == GLFW_PRESS )
    {
        //if( mods & GLFW_MOD_CONTROL && key == GLFW_KEY_S )
        if( key == GLFW_KEY_F4 )
        {
            simulationPtr->SavePGM( "erosion.pgm" );
        }
    }
}

int main(int argc, char** argv)
{
    // Settings ////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////

    int windowWidth = 800;
    int windowHeight = 600;
    uint terrainDim = 300;

    // Read Command Line Arguments /////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////
    try
    {
        TCLAP::CmdLine cmd("Terrain Eroision & Fluid Simulation.", ' ', "0.9");
        TCLAP::ValueArg<uint> dimArg("d","dim","Size of the terrain. Default: 300.",false,300,"uint");
        cmd.add(dimArg);
        cmd.parse( argc, argv );
        terrainDim = dimArg.getValue();
    }
    catch (TCLAP::ArgException &e)
    {
        std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
    }


    // Open GL Stuff ///////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////

    // init GLFW
    if (!glfwInit())
    {
        std::cerr << "[GLFW] Error initialising GLFW" << std::endl;
        exit(1);
    }


    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
    // Use OpenGL Core v3.2
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Open an OpenGL window
    win = glfwCreateWindow( windowWidth, windowHeight, "Terrain", NULL, NULL );
    if( ! win )
    {
        std::cerr << "[GLFW] Error opening window" << std::endl;
        glfwTerminate();
        exit(1);
    }

    glfwMakeContextCurrent( win );

    int major, minor, rev;

    major = glfwGetWindowAttrib( win, GLFW_CONTEXT_VERSION_MAJOR );
    minor = glfwGetWindowAttrib( win, GLFW_CONTEXT_VERSION_MINOR );
    rev   = glfwGetWindowAttrib( win, GLFW_CONTEXT_REVISION );


    fprintf(stdout, "OpenGL version recieved: %d.%d.%d\n", major, minor, rev);
    // Init Glew (OpenGL Extension Loading)

#if defined(__APPLE__) || defined(__MACH__)
    // Do nothing
#else
    // http://stackoverflow.com/questions/8302625/segmentation-fault-at-glgenvertexarrays-1-vao
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        std::cerr << "[GLEW] Init failed: " << glewGetErrorString(err) << std::endl;
        glfwTerminate();
        exit(1);
    }
#endif
    // Start Simulation ////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////

    glfwSetWindowCloseCallback(win, onWindowClose);
    glfwSetKeyCallback(win, keyHandler);


    simulationPtr = new TerrainFluidSimulation(terrainDim);
    simulationPtr->Run();

    delete simulationPtr;
    glfwDestroyWindow( win );
    glfwTerminate();

    cout << "The end." << endl;
    return 0;
}





