/*
Name: LAI Man Hin
SID: 1155136167
CSCI3260 Assg1
*/

#include "Dependencies/glew/glew.h"
#include "Dependencies/GLFW/glfw3.h"

#include "Dependencies/glm/glm.hpp"
#include "Dependencies/glm/gtc/matrix_transform.hpp"

#include <iostream>
#include <fstream>


GLuint programID;
float x_delta = 0.1f;
int x_press_num = 0;
float flagY = 0.0, camYZ = 0.0, camXZ = 0.0, zoom = 11.0f, sunSize = 1.0, fov = 45.0;
int open = 0;
GLuint platVao;
GLuint groundVao;
GLuint poleVao;
GLuint flagVao;
GLuint gateAVao;
GLuint gateBVao;
GLuint sunVao;
GLuint cloudVao;

void get_OpenGL_info() {
    // OpenGL information
    const GLubyte* name = glGetString(GL_VENDOR);
    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* glversion = glGetString(GL_VERSION);
    std::cout << "OpenGL company: " << name << std::endl;
    std::cout << "Renderer name: " << renderer << std::endl;
    std::cout << "OpenGL version: " << glversion << std::endl;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void sendDataToOpenGL() {
    const GLfloat plat[] =
    {
        -2.5f, -0.15f, -1.0f, +1.0f, +1.0f, +1.0f,   //0

        +2.5f, -0.15f, -1.0f, +1.0f, +0.0f, +1.0f,   //1

        +2.5f, +0.15f, -1.0f, +1.0f, +1.0f, +0.0f,   //2

        -2.5f, +0.15f, -1.0f, +1.0f, +1.0f, +0.0f,   //3

        -2.5f, -0.15f, +1.0f, +1.0f, +0.0f, +1.0f,   //4

        +2.5f, -0.15f, +1.0f, +0.0f, +1.0f, +1.0f,   //5

        +2.5f, +0.15f, +1.0f, +1.0f, +0.0f, +1.0f,   //6

        -2.5f, +0.15f, +1.0f, +1.0f, +1.0f, +0.0f,   //7

    };

    
    glGenVertexArrays(1, &platVao);
    glBindVertexArray(platVao);  //first VAO

    GLuint platVbo;
    glGenBuffers(1, &platVbo);
    glBindBuffer(GL_ARRAY_BUFFER, platVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(plat), plat, GL_STATIC_DRAW);

    // vertex position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    // vertex color
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));
    
    //// with indexing (uncomment to use)
    GLuint platInd[] = { 0, 1, 2,
        0, 2, 3,
        4, 5, 6, 
        4, 6, 7,
        0, 1, 5, 
        0, 4, 5,
        2, 3, 6,
        3, 6, 7,
        0, 3, 7, 
        0, 4, 7,
        1, 2, 6, 
        1, 5, 6};

    //// index buffer
    GLuint platID;
    glGenBuffers(1, &platID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, platID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(platInd), platInd, GL_STATIC_DRAW);

    const GLfloat ground[] = {

        +4.0f, -2.0f, +4.0f, 101/255.0f, 67/255.0f, 33/255.0f,   //0

        -4.0f, -2.0f, +4.0f, 101 / 255.0f, 67 / 255.0f, 33 / 255.0f,   //1

        -4.0f, -2.0f, -4.0f, 101 / 255.0f, 67 / 255.0f, 33 / 255.0f,   //2

        +4.0f, -2.0f, -4.0f, 101 / 255.0f, 67 / 255.0f, 33 / 255.0f,   //3
    };

    
    glGenVertexArrays(1, &groundVao);
    glBindVertexArray(groundVao);

    GLuint groundVbo;
    glGenBuffers(1, &groundVbo);
    glBindBuffer(GL_ARRAY_BUFFER, groundVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ground), ground, GL_STATIC_DRAW);

    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));

    GLuint groundInd[] = { 0, 1, 2,
        0, 2, 3 };

    GLuint groundID;
    glGenBuffers(1, &groundID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, groundID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(groundInd), groundInd, GL_STATIC_DRAW);

    const GLfloat pole[] =
    {
        -0.1f, -0.5f, -0.1f, +1.0f, +1.0f, +1.0f,   //0

        +0.1f, -0.5f, -0.1f, +1.0f, +1.0f, +1.0f,   //1

        +0.1f, +6.5f, -0.1f, +1.0f, +1.0f, +1.0f,   //2

        -0.1f, +6.5f, -0.1f, +1.0f, +1.0f, +1.0f,   //3

        -0.1f, -0.5f, +0.1f, +1.0f, +1.0f, +1.0f,   //4

        +0.1f, -0.5f, +0.1f, +1.0f, +1.0f, +1.0f,   //5

        +0.1f, +6.5f, +0.1f, +1.0f, +1.0f, +1.0f,   //6

        -0.1f, +6.5f, +0.1f, +1.0f, +1.0f, +1.0f,   //7

    };


    glGenVertexArrays(1, &poleVao);
    glBindVertexArray(poleVao);

    GLuint poleVbo;
    glGenBuffers(1, &poleVbo);
    glBindBuffer(GL_ARRAY_BUFFER, poleVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pole), pole, GL_STATIC_DRAW);


    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));

    GLuint poleInd[] = { 0, 1, 2,
        0, 2, 3,
        4, 5, 6,
        4, 6, 7,
        0, 1, 5,
        0, 4, 5,
        2, 3, 6,
        3, 6, 7,
        0, 3, 7,
        0, 4, 7,
        1, 2, 6,
        1, 5, 6 };

    GLuint poleID;
    glGenBuffers(1, &poleID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, poleID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(poleInd), poleInd, GL_STATIC_DRAW);

    const GLfloat flag[] = {

        +1.5f, -2.0f, -2.0f, 1.0f, 0.0f, 0.0f,   //0

        -1.5f, -2.0f, -2.0f, 1.0f, 0.0f, 1.0f,   //1

        -1.5f, -0.0f, -2.0f, 0.0f, 1.0f, 0.0f,   //2

        +1.5f, -0.0f, -2.0f, 1.0f, 0.0f, 1.0f   //3
    };


    glGenVertexArrays(1, &flagVao);
    glBindVertexArray(flagVao);

    GLuint flagVbo;
    glGenBuffers(1, &flagVbo);
    glBindBuffer(GL_ARRAY_BUFFER, flagVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(flag), flag, GL_STATIC_DRAW);


    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));

    GLuint flagInd[] = { 0, 1, 2,
        0, 2, 3 };

    GLuint flagID;
    glGenBuffers(1, &flagID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, flagID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(flagInd), flagInd, GL_STATIC_DRAW);

    const GLfloat gateA[] = {

        -2.5f, -2.5f, +0.25f, 0.6f, 0.6f, 0.6f,   //0

        +2.0f, -2.5f, +0.25f, 0.6f, 0.6f, 0.6f,   //1

        +2.0f, -2.5f, -0.25f, 0.6f, 0.6f, 0.6f,   //2

        -2.5f, -2.5f, -0.25f, 0.6f, 0.6f, 0.6f,   //3

        -2.5f, -2.0f, +0.25f, 0.6f, 0.6f, 0.6f,   //4

        +2.0f, -2.0f, +0.25f, 0.6f, 0.6f, 0.6f,   //5

        +2.0f, -2.0f, -0.25f, 0.6f, 0.6f, 0.6f,   //6

        -2.5f, -2.0f, -0.25f, 0.6f, 0.6f, 0.6f,   //7

        -2.5f, +2.5f, +0.25f, 0.6f, 0.6f, 0.6f,   //8

        +2.0f, +2.5f, +0.25f, 0.6f, 0.6f, 0.6f,   //9

        +2.0f, +2.5f, -0.25f, 0.6f, 0.6f, 0.6f,   //10

        -2.5f, +2.5f, -0.25f, 0.6f, 0.6f, 0.6f,   //11

        -2.5f, +2.0f, +0.25f, 0.6f, 0.6f, 0.6f,   //12

        +2.0f, +2.0f, +0.25f, 0.6f, 0.6f, 0.6f,   //13

        +2.0f, +2.0f, -0.25f, 0.6f, 0.6f, 0.6f,   //14

        -2.5f, +2.0f, -0.25f, 0.6f, 0.6f, 0.6f,   //15

        +1.5f, +2.0f, +0.25f, 0.6f, 0.6f, 0.6f,   //16

        +1.5f, +2.0f, -0.25f, 0.6f, 0.6f, 0.6f,   //17

        +1.5f, -2.0f, -0.25f, 0.6f, 0.6f, 0.6f,   //18

        +1.5f, -2.0f, +0.25f, 0.6f, 0.6f, 0.6f,   //19

        +1.0f, +2.0f, +0.25f, 0.6f, 0.6f, 0.6f,   //20

        +1.0f, +2.0f, -0.25f, 0.6f, 0.6f, 0.6f,   //21

        +1.0f, -2.0f, -0.25f, 0.6f, 0.6f, 0.6f,   //22

        +1.0f, -2.0f, +0.25f, 0.6f, 0.6f, 0.6f,   //23

        +0.5f, +2.0f, +0.25f, 0.6f, 0.6f, 0.6f,   //24

        +0.5f, +2.0f, -0.25f, 0.6f, 0.6f, 0.6f,   //25

        +0.5f, -2.0f, -0.25f, 0.6f, 0.6f, 0.6f,   //26

        +0.5f, -2.0f, +0.25f, 0.6f, 0.6f, 0.6f,   //27

        -0.0f, +2.0f, +0.25f, 0.6f, 0.6f, 0.6f,   //28

        -0.0f, +2.0f, -0.25f, 0.6f, 0.6f, 0.6f,   //29

        -0.0f, -2.0f, -0.25f, 0.6f, 0.6f, 0.6f,   //30

        -0.0f, -2.0f, +0.25f, 0.6f, 0.6f, 0.6f,   //31

        -0.5f, +2.0f, +0.25f, 0.6f, 0.6f, 0.6f,   //32

        -0.5f, +2.0f, -0.25f, 0.6f, 0.6f, 0.6f,   //33

        -0.5f, -2.0f, -0.25f, 0.6f, 0.6f, 0.6f,   //34

        -0.5f, -2.0f, +0.25f, 0.6f, 0.6f, 0.6f,   //35

        -1.0f, +2.0f, +0.25f, 0.6f, 0.6f, 0.6f,   //36

        -1.0f, +2.0f, -0.25f, 0.6f, 0.6f, 0.6f,   //37

        -1.0f, -2.0f, -0.25f, 0.6f, 0.6f, 0.6f,   //38

        -1.0f, -2.0f, +0.25f, 0.6f, 0.6f, 0.6f,   //39

        -1.5f, +2.0f, +0.25f, 0.6f, 0.6f, 0.6f,   //40

        -1.5f, +2.0f, -0.25f, 0.6f, 0.6f, 0.6f,   //41

        -1.5f, -2.0f, -0.25f, 0.6f, 0.6f, 0.6f,   //42

        -1.5f, -2.0f, +0.25f, 0.6f, 0.6f, 0.6f,   //43

        -2.0f, +2.0f, +0.25f, 0.6f, 0.6f, 0.6f,   //44

        -2.0f, +2.0f, -0.25f, 0.6f, 0.6f, 0.6f,   //45

        -2.0f, -2.0f, -0.25f, 0.6f, 0.6f, 0.6f,   //46

        -2.0f, -2.0f, +0.25f, 0.6f, 0.6f, 0.6f   //47

    };


    glGenVertexArrays(1, &gateAVao);
    glBindVertexArray(gateAVao);

    GLuint gateAVbo;
    glGenBuffers(1, &gateAVbo);
    glBindBuffer(GL_ARRAY_BUFFER, gateAVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(gateA), gateA, GL_STATIC_DRAW);


    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));

    GLuint gateAInd[] = { 0, 1, 3,
        1, 2, 3,
        4, 5, 7,
        5, 6, 7,
        0, 1, 5,
        1, 4, 5,
        2, 3, 6,
        3, 6, 7,
        1, 2, 6,
        2, 5, 6,
        0, 3, 4,
        3, 4, 7,
        8, 9, 11,
        9, 10, 11,
        12, 13, 15,
        13, 14, 15,
        8, 9, 13,
        9, 12, 13,
        10, 11, 14,
        11, 14, 15,
        9, 10, 14,
        10, 13, 14,
        8, 11, 12,
        11, 12, 15,
        5, 6, 13,
        6, 13, 14,
        4, 7, 12,
        7, 12, 15,
        7,45, 15, 
        7, 45, 46, 
        44, 47, 4,
        44, 12, 4,
        13, 19, 5,
        13, 19, 16,
        14, 18, 6,
        14, 18, 17,
        20, 27, 23,
        20, 27, 24,
        21, 26, 22,
        21, 26, 25,
        28, 35, 31,
        28, 35, 32,
        29, 34, 30,
        29, 34, 33,
        36, 43, 39,
        36, 43, 40,
        37, 42, 38,
        37, 42, 41,
        16, 18, 17,
        16, 18, 19,
        20, 22, 21,
        20, 22, 23,
        24, 26, 25,
        24, 26, 27,
        28, 30, 29,
        28, 30, 31,
        32, 34, 33,
        32, 34, 35,
        36, 38, 37,
        36, 38, 39,
        40, 42, 41,
        40, 42, 43,
        44, 46, 45,
        44, 46, 47
    };

    GLuint gateAID;
    glGenBuffers(1, &gateAID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gateAID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(gateAInd), gateAInd, GL_STATIC_DRAW);

    const GLfloat gateB[] = {

        -2.5f, -2.5f, +0.25f, 0.6f, 0.6f, 0.6f,   //0

        +2.0f, -2.5f, +0.25f, 0.6f, 0.6f, 0.6f,   //1

        +2.0f, -2.5f, -0.25f, 0.6f, 0.6f, 0.6f,   //2

        -2.5f, -2.5f, -0.25f, 0.6f, 0.6f, 0.6f,   //3

        -2.5f, -2.0f, +0.25f, 0.6f, 0.6f, 0.6f,   //4

        +2.0f, -2.0f, +0.25f, 0.6f, 0.6f, 0.6f,   //5

        +2.0f, -2.0f, -0.25f, 0.6f, 0.6f, 0.6f,   //6

        -2.5f, -2.0f, -0.25f, 0.6f, 0.6f, 0.6f,   //7

        -2.5f, +2.5f, +0.25f, 0.6f, 0.6f, 0.6f,   //8

        +2.0f, +2.5f, +0.25f, 0.6f, 0.6f, 0.6f,   //9

        +2.0f, +2.5f, -0.25f, 0.6f, 0.6f, 0.6f,   //10

        -2.5f, +2.5f, -0.25f, 0.6f, 0.6f, 0.6f,   //11

        -2.5f, +2.0f, +0.25f, 0.6f, 0.6f, 0.6f,   //12

        +2.0f, +2.0f, +0.25f, 0.6f, 0.6f, 0.6f,   //13

        +2.0f, +2.0f, -0.25f, 0.6f, 0.6f, 0.6f,   //14

        -2.5f, +2.0f, -0.25f, 0.6f, 0.6f, 0.6f,   //15

        +1.5f, +2.0f, +0.25f, 0.6f, 0.6f, 0.6f,   //16

        +1.5f, +2.0f, -0.25f, 0.6f, 0.6f, 0.6f,   //17

        +1.5f, -2.0f, -0.25f, 0.6f, 0.6f, 0.6f,   //18

        +1.5f, -2.0f, +0.25f, 0.6f, 0.6f, 0.6f,   //19

        +1.0f, +2.0f, +0.25f, 0.6f, 0.6f, 0.6f,   //20

        +1.0f, +2.0f, -0.25f, 0.6f, 0.6f, 0.6f,   //21

        +1.0f, -2.0f, -0.25f, 0.6f, 0.6f, 0.6f,   //22

        +1.0f, -2.0f, +0.25f, 0.6f, 0.6f, 0.6f,   //23

        +0.5f, +2.0f, +0.25f, 0.6f, 0.6f, 0.6f,   //24

        +0.5f, +2.0f, -0.25f, 0.6f, 0.6f, 0.6f,   //25

        +0.5f, -2.0f, -0.25f, 0.6f, 0.6f, 0.6f,   //26

        +0.5f, -2.0f, +0.25f, 0.6f, 0.6f, 0.6f,   //27

        -0.0f, +2.0f, +0.25f, 0.6f, 0.6f, 0.6f,   //28

        -0.0f, +2.0f, -0.25f, 0.6f, 0.6f, 0.6f,   //29

        -0.0f, -2.0f, -0.25f, 0.6f, 0.6f, 0.6f,   //30

        -0.0f, -2.0f, +0.25f, 0.6f, 0.6f, 0.6f,   //31

        -0.5f, +2.0f, +0.25f, 0.6f, 0.6f, 0.6f,   //32

        -0.5f, +2.0f, -0.25f, 0.6f, 0.6f, 0.6f,   //33

        -0.5f, -2.0f, -0.25f, 0.6f, 0.6f, 0.6f,   //34

        -0.5f, -2.0f, +0.25f, 0.6f, 0.6f, 0.6f,   //35

        -1.0f, +2.0f, +0.25f, 0.6f, 0.6f, 0.6f,   //36

        -1.0f, +2.0f, -0.25f, 0.6f, 0.6f, 0.6f,   //37

        -1.0f, -2.0f, -0.25f, 0.6f, 0.6f, 0.6f,   //38

        -1.0f, -2.0f, +0.25f, 0.6f, 0.6f, 0.6f,   //39

        -1.5f, +2.0f, +0.25f, 0.6f, 0.6f, 0.6f,   //40

        -1.5f, +2.0f, -0.25f, 0.6f, 0.6f, 0.6f,   //41

        -1.5f, -2.0f, -0.25f, 0.6f, 0.6f, 0.6f,   //42

        -1.5f, -2.0f, +0.25f, 0.6f, 0.6f, 0.6f,   //43

        -2.0f, +2.0f, +0.25f, 0.6f, 0.6f, 0.6f,   //44

        -2.0f, +2.0f, -0.25f, 0.6f, 0.6f, 0.6f,   //45

        -2.0f, -2.0f, -0.25f, 0.6f, 0.6f, 0.6f,   //46

        -2.0f, -2.0f, +0.25f, 0.6f, 0.6f, 0.6f   //47

    };


    glGenVertexArrays(1, &gateBVao);
    glBindVertexArray(gateBVao);

    GLuint gateBVbo;
    glGenBuffers(1, &gateBVbo);
    glBindBuffer(GL_ARRAY_BUFFER, gateBVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(gateB), gateB, GL_STATIC_DRAW);

    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));

    GLuint gateBInd[] = { 0, 1, 3,
        1, 2, 3,
        4, 5, 7,
        5, 6, 7,
        0, 1, 5,
        1, 4, 5,
        2, 3, 6,
        3, 6, 7,
        1, 2, 6,
        2, 5, 6,
        0, 3, 4,
        3, 4, 7,
        8, 9, 11,
        9, 10, 11,
        12, 13, 15,
        13, 14, 15,
        8, 9, 13,
        9, 12, 13,
        10, 11, 14,
        11, 14, 15,
        9, 10, 14,
        10, 13, 14,
        8, 11, 12,
        11, 12, 15,
        5, 6, 13,
        6, 13, 14,
        4, 7, 12,
        7, 12, 15,
        7,45, 15,
        7, 45, 46,
        44, 47, 4,
        44, 12, 4,
        13, 19, 5,
        13, 19, 16,
        14, 18, 6,
        14, 18, 17,
        20, 27, 23,
        20, 27, 24,
        21, 26, 22,
        21, 26, 25,
        28, 35, 31,
        28, 35, 32,
        29, 34, 30,
        29, 34, 33,
        36, 43, 39,
        36, 43, 40,
        37, 42, 38,
        37, 42, 41,
        16, 18, 17,
        16, 18, 19,
        20, 22, 21,
        20, 22, 23,
        24, 26, 25,
        24, 26, 27,
        28, 30, 29,
        28, 30, 31,
        32, 34, 33,
        32, 34, 35,
        36, 38, 37,
        36, 38, 39,
        40, 42, 41,
        40, 42, 43,
        44, 46, 45,
        44, 46, 47
    };

    GLuint gateBID;
    glGenBuffers(1, &gateBID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gateBID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(gateBInd), gateBInd, GL_STATIC_DRAW);

    const GLfloat sun[] = {

        +0.5f, +1.0f, +0.5f, 253.0f / 255, 184.0f / 255, 19.0f / 255,   //0

        -0.5f, +1.0f, +0.5f, 253.0f / 255, 184.0f / 255, 19.0f / 255,   //1

        -0.5f, +1.0f, -0.5f, 253.0f / 255, 184.0f / 255, 19.0f / 255,   //2

        +0.5f, +1.0f, -0.5f, 253.0f / 255, 184.0f / 255, 19.0f / 255,   //3

        +1.0f, +0.5f, +1.0f, 253.0f / 255, 184.0f / 255, 19.0f / 255,   //4

        -1.0f, +0.5f, +1.0f, 253.0f / 255, 184.0f / 255, 19.0f / 255,   //5

        -1.0f, +0.5f, -1.0f, 253.0f / 255, 184.0f / 255, 19.0f / 255,   //6

        +1.0f, +0.5f, -1.0f, 253.0f / 255, 184.0f / 255, 19.0f / 255,   //7

        +1.0f, -0.5f, +1.0f, 253.0f / 255, 184.0f / 255, 19.0f / 255,   //8

        -1.0f, -0.5f, +1.0f, 253.0f / 255, 184.0f / 255, 19.0f / 255,   //9

        -1.0f, -0.5f, -1.0f, 253.0f / 255, 184.0f / 255, 19.0f / 255,   //10

        +1.0f, -0.5f, -1.0f, 253.0f / 255, 184.0f / 255, 19.0f / 255,   //11

        +0.5f, -1.0f, +0.5f, 253.0f / 255, 184.0f / 255, 19.0f / 255,   //12

        -0.5f, -1.0f, +0.5f, 253.0f / 255, 184.0f / 255, 19.0f / 255,   //13

        -0.5f, -1.0f, -0.5f, 253.0f / 255, 184.0f / 255, 19.0f / 255,   //14

        +0.5f, -1.0f, -0.5f, 253.0f / 255, 184.0f / 255, 19.0f / 255,   //15

    };


    glGenVertexArrays(1, &sunVao);
    glBindVertexArray(sunVao);

    GLuint sunVbo;
    glGenBuffers(1, &sunVbo);
    glBindBuffer(GL_ARRAY_BUFFER, sunVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(sun), sun, GL_STATIC_DRAW);


    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));

    GLuint sunInd[] = { 0, 1, 3,
        1, 3, 2,
        0, 5, 1,
        0, 5, 4,
        1, 6, 2,
        1, 6, 5,
        2, 7, 3,
        2, 7, 6,
        0, 7, 3,
        0, 7, 4,
        4, 9, 5,
        4, 9, 8,
        5, 10, 6,
        5, 10, 9,
        6, 11, 7,
        6, 11, 10,
        7, 8, 4,
        7, 8, 11,
        8, 13, 9,
        8, 13, 12,
        9, 14, 10,
        9, 14, 13,
        10, 15, 11,
        10, 15, 14,
        11, 12, 8,
        11, 12, 15,
        12, 14, 13,
        12, 14, 15};

    GLuint sunID;
    glGenBuffers(1, &sunID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sunID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sunInd), sunInd, GL_STATIC_DRAW);

    const GLfloat cloud[] =
    {
        -0.5f, -0.5f, -0.5f, +1.0f, +1.0f, +1.0f,   //0

        +1.5f, -0.5f, -0.5f, +1.0f, +1.0f, +1.0f,   //1

        +1.5f, +0.5f, -0.5f, +1.0f, +1.0f, +1.0f,   //2

        -0.5f, +0.5f, -0.5f, +1.0f, +1.0f, +1.0f,   //3

        -0.5f, -0.5f, +0.5f, +1.0f, +1.0f, +1.0f,   //4

        +1.5f, -0.5f, +0.5f, +1.0f, +1.0f, +1.0f,   //5

        +1.5f, +0.5f, +0.5f, +1.0f, +1.0f, +1.0f,   //6

        -0.5f, +0.5f, +0.5f, +1.0f, +1.0f, +1.0f,   //7

        +1.5f, +0.0f, +0.5f, +1.0f, +1.0f, +1.0f,   //8

        +1.5f, -0.5f, +0.5f, +1.0f, +1.0f, +1.0f,   //9

        +1.5f, -0.5f, +0.0f, +1.0f, +1.0f, +1.0f,   //10

        +1.5f, +0.0f, +0.0f, +1.0f, +1.0f, +1.0f,   //11

        +2.2f, +0.0f, +0.5f, +1.0f, +1.0f, +1.0f,   //12

        +2.2f, -0.5f, +0.5f, +1.0f, +1.0f, +1.0f,   //13

        +2.2f, -0.5f, +0.0f, +1.0f, +1.0f, +1.0f,   //14

        +2.2f, +0.0f, +0.0f, +1.0f, +1.0f, +1.0f,   //15

    };


    glGenVertexArrays(1, &cloudVao);
    glBindVertexArray(cloudVao);  //first VAO

    GLuint cloudVbo;
    glGenBuffers(1, &cloudVbo);
    glBindBuffer(GL_ARRAY_BUFFER, cloudVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cloud), cloud, GL_STATIC_DRAW);


    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));

    GLuint cloudInd[] = { 0, 1, 2,
        0, 2, 3,
        4, 5, 6,
        4, 6, 7,
        0, 1, 5,
        0, 4, 5,
        2, 3, 6,
        3, 6, 7,
        0, 3, 7,
        0, 4, 7,
        1, 2, 6,
        1, 5, 6,
        12, 14, 13,
        12, 14, 15,
        8, 15, 11,
        8, 15, 12,
        10, 13, 9,
        10, 13, 14,
        9, 12, 8,
        9, 12, 13,
        10, 15, 11,
        10, 15, 14
    };

    GLuint cloudID;
    glGenBuffers(1, &cloudID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cloudID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cloudInd), cloudInd, GL_STATIC_DRAW);
}

bool checkStatus(
    GLuint objectID,
    PFNGLGETSHADERIVPROC objectPropertyGetterFunc,
    PFNGLGETSHADERINFOLOGPROC getInfoLogFunc,
    GLenum statusType)
{
    GLint status;
    objectPropertyGetterFunc(objectID, statusType, &status);
    if (status != GL_TRUE)
    {
        GLint infoLogLength;
        objectPropertyGetterFunc(objectID, GL_INFO_LOG_LENGTH, &infoLogLength);
        GLchar* buffer = new GLchar[infoLogLength];

        GLsizei bufferSize;
        getInfoLogFunc(objectID, infoLogLength, &bufferSize, buffer);
        std::cout << buffer << std::endl;

        delete[] buffer;
        return false;
    }
    return true;
}

bool checkShaderStatus(GLuint shaderID) {
    return checkStatus(shaderID, glGetShaderiv, glGetShaderInfoLog, GL_COMPILE_STATUS);
}

bool checkProgramStatus(GLuint programID) {
    return checkStatus(programID, glGetProgramiv, glGetProgramInfoLog, GL_LINK_STATUS);
}

std::string readShaderCode(const char* fileName) {
    std::ifstream meInput(fileName);
    if (!meInput.good()) {
        std::cout << "File failed to load ... " << fileName << std::endl;
        exit(1);
    }
    return std::string(
        std::istreambuf_iterator<char>(meInput),
        std::istreambuf_iterator<char>()
    );
}

void installShaders() {
    GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    const GLchar* adapter[1];
    //adapter[0] = vertexShaderCode;
    std::string temp = readShaderCode("VertexShaderCode.glsl");
    adapter[0] = temp.c_str();
    glShaderSource(vertexShaderID, 1, adapter, 0);
    //adapter[0] = fragmentShaderCode;
    temp = readShaderCode("FragmentShaderCode.glsl");
    adapter[0] = temp.c_str();
    glShaderSource(fragmentShaderID, 1, adapter, 0);

    glCompileShader(vertexShaderID);
    glCompileShader(fragmentShaderID);

    if (!checkShaderStatus(vertexShaderID) || !checkShaderStatus(fragmentShaderID))
        return;

    programID = glCreateProgram();
    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);
    glLinkProgram(programID);

    if (!checkProgramStatus(programID))
        return;
    glUseProgram(programID);

}

void initializedGL(void) {
    // run only once
    sendDataToOpenGL();
    installShaders();
}

void paintGL(void) {
    // always run
    glClearColor(0.1f, 0.85f, 0.95f, 1.0f);  //specify the background color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f),
        glm::vec3(0.0f, 0.0f, 0.0f));
    GLint modelMatrixUniformLocation = glGetUniformLocation(programID, "modelMatrix");
    glUniformMatrix4fv(modelMatrixUniformLocation, 1, GL_FALSE, &modelMatrix[0][0]);

    
    
    glm::mat4 viewMatrix = glm::lookAt(glm::vec3(zoom * sin(glm::radians(camXZ)), zoom * sin(glm::radians(camYZ)), zoom * cos(glm::radians(camYZ)) * cos(glm::radians(camXZ))),
        glm::vec3(0.0f, 0.0f, 0.0f), 
        glm::vec3(0.0f, 1.0f, 0.0f));
    GLint viewMatrixUniformLocation = glGetUniformLocation(programID, "viewMatrix");
    glUniformMatrix4fv(viewMatrixUniformLocation, 1, GL_FALSE, &viewMatrix[0][0]);
   

    glm::mat4 projectionMatrix = glm::perspective(glm::radians(fov), 1.0f, 0.1f, 20.0f);
    GLint projectionMatrixUniformLocation = glGetUniformLocation(programID, "projectionMatrix");
    glUniformMatrix4fv(projectionMatrixUniformLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
    glBindVertexArray(groundVao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    modelMatrix = glm::translate(glm::mat4(1.0f),
        glm::vec3(0.0f, -1.80f, -2.5f));
    modelMatrixUniformLocation = glGetUniformLocation(programID, "modelMatrix");
    glUniformMatrix4fv(modelMatrixUniformLocation, 1, GL_FALSE, &modelMatrix[0][0]);
    glBindVertexArray(platVao);

    // without indexing
    //glDrawArrays(GL_TRIANGLES, 0, 6);  //render primitives from array data
    //// with indexing (uncomment to use)
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    modelMatrix = glm::translate(glm::mat4(1.0f),
        glm::vec3(0.0f, -1.5f, -3.0f));
    modelMatrixUniformLocation = glGetUniformLocation(programID, "modelMatrix");
    glUniformMatrix4fv(modelMatrixUniformLocation, 1, GL_FALSE, &modelMatrix[0][0]);
    glBindVertexArray(poleVao);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    modelMatrix = glm::translate(glm::mat4(1.0f),
        glm::vec3(1.5f, 1.0f + flagY, -1.0f));
    modelMatrixUniformLocation = glGetUniformLocation(programID, "modelMatrix");
    glUniformMatrix4fv(modelMatrixUniformLocation, 1, GL_FALSE, &modelMatrix[0][0]);
    glBindVertexArray(flagVao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    modelMatrix = glm::translate(glm::mat4(1.0f),
        glm::vec3(-1.8f, 0.20f, 0.0f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(2.6f / 3));
    if (open == 1)
    {
        modelMatrix = glm::translate(modelMatrix,
            glm::vec3(-2.25f, +0.0f, +2.0f));
        modelMatrix = glm::rotate(modelMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    }
    modelMatrixUniformLocation = glGetUniformLocation(programID, "modelMatrix");
    glUniformMatrix4fv(modelMatrixUniformLocation, 1, GL_FALSE, &modelMatrix[0][0]);
    glBindVertexArray(gateAVao);
    glDrawElements(GL_TRIANGLES,192, GL_UNSIGNED_INT, 0);

    modelMatrix = glm::translate(glm::mat4(1.0f),
        glm::vec3(+2.2f, +0.20f, 0.0f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(2.6f / 3));
    if (open == 1)
    {
        modelMatrix = glm::translate(modelMatrix,
            glm::vec3(+1.75f, +0.0f, +2.0f));
        modelMatrix = glm::rotate(modelMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        
    }
    modelMatrixUniformLocation = glGetUniformLocation(programID, "modelMatrix");
    glUniformMatrix4fv(modelMatrixUniformLocation, 1, GL_FALSE, &modelMatrix[0][0]);
    glBindVertexArray(gateBVao);
    glDrawElements(GL_TRIANGLES, 192, GL_UNSIGNED_INT, 0);
    
    modelMatrix = glm::translate(glm::mat4(1.0f),
        glm::vec3(4.0f, 7.0f, -6.0f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(sunSize));
    modelMatrixUniformLocation = glGetUniformLocation(programID, "modelMatrix");
    glUniformMatrix4fv(modelMatrixUniformLocation, 1, GL_FALSE, &modelMatrix[0][0]);
    glBindVertexArray(sunVao);
    glDrawElements(GL_TRIANGLES, 84, GL_UNSIGNED_INT, 0);

    modelMatrix = glm::translate(glm::mat4(1.0f),
        glm::vec3(-4.0f, 7.0f, -6.0f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(1.5f));
    modelMatrixUniformLocation = glGetUniformLocation(programID, "modelMatrix");
    glUniformMatrix4fv(modelMatrixUniformLocation, 1, GL_FALSE, &modelMatrix[0][0]);
    glBindVertexArray(cloudVao);
    glDrawElements(GL_TRIANGLES, 66, GL_UNSIGNED_INT, 0);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (key == GLFW_KEY_W && action == GLFW_PRESS) {
        camYZ += 5.0;
    }
    if (key == GLFW_KEY_S && action == GLFW_PRESS) {
        camYZ -= 5.0;
    }
    if (key == GLFW_KEY_A && action == GLFW_PRESS) {
        camXZ -= 5.0;
    }
    if (key == GLFW_KEY_D && action == GLFW_PRESS) {
        camXZ += 5.0;
    }
    if (key == GLFW_KEY_EQUAL && action == GLFW_PRESS) {
        zoom += 0.25;
    }
    if (key == GLFW_KEY_MINUS && action == GLFW_PRESS) {
        zoom -= 0.25;
    }
    if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
        if (flagY < 4)
            flagY += 0.25;
    }
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
        if (flagY > 0)
            flagY -= 0.25;
    }
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        open = (open + 1) % 2;
    }
    if (key == GLFW_KEY_J && action == GLFW_PRESS) {
        if (sunSize > 1.0)
            sunSize -= 0.2f;
    }
    if (key == GLFW_KEY_K && action == GLFW_PRESS) {
        if (sunSize < 2.0)
            sunSize += 0.2f;
    }
    if (key == GLFW_KEY_N && action == GLFW_PRESS) {
        fov = 45.0;
    }
    if (key == GLFW_KEY_M && action == GLFW_PRESS) {
        fov = 60.0;
    }
}

int main(int argc, char* argv[]) {
    GLFWwindow* window;

    /* Initialize the glfw */
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    /* glfw: configure; necessary for MAC */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    /* do not allow resizing */
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(512, 512, argv[0], NULL, NULL);
    if (!window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);

    /* Initialize the glew */
    if (GLEW_OK != glewInit()) {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }
    get_OpenGL_info();
    initializedGL();
    //enable depth test
    glEnable(GL_DEPTH_TEST);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        /* Render here */
        paintGL();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}