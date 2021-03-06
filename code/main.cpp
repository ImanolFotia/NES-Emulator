#include <iostream>
#include <fstream>
#include <memory>
#include <cstdlib>

#include "include/memory.h"
#include "include/CPU.h"
#define GLEW STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

const unsigned MAX_SYSTEM_MEMORY = 0xffff;

/**
The format of the header is as follows:
0-3: Constant $4E $45 $53 $1A ("NES" followed by MS-DOS end-of-file)
4: Size of PRG ROM in 16 KB units
5: Size of CHR ROM in 8 KB units (Value 0 means the board uses CHR RAM)
6: Flags 6
7: Flags 7
8: Size of PRG RAM in 8 KB units (Value 0 infers 8 KB for compatibility; see PRG RAM circuit)
9: Flags 9
10: Flags 10 (unofficial)
11-15: Zero filled
*/

int main(int argc, char** argv)
{
    std::shared_ptr<CPU::CPU6502> _CPU = (std::shared_ptr<CPU::CPU6502>) new CPU::CPU6502();

    std::cout << "Loading INES ROM..." << std::endl;
    std::ifstream NESROM(std::string(argv[1]), std::ios::binary);

    unsigned char* buffer = new unsigned char[16];
    NESROM.read((char*)buffer, 16);

    if(buffer[0] != 'N' || buffer[1] != 'E' || buffer[2] != 'S') {
        std::cout << "Error: Not a NES ROM file. Aborting." << std::endl;
        std::exit(1);
    }

    std::cout << "Valid INES File" << std::endl;

    std::cout << "PRG ROM: " << buffer[4] * 16348 << std::endl;
    std::cout << "CHR ROM: " << buffer[5] * 8192 << std::endl;
    std::cout << "PRG RAM: " << buffer[8] * 8192 << std::endl;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(256, 240, "NES Emulator", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glewExperimental = true;
    glewInit();


    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.3, 0.6, 0.3, 1.0);

        glfwSwapBuffers(window);
    }

    glfwTerminate();

    return 0;
}
