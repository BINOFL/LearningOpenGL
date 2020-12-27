//
//  Person.cpp
//  learningOpenGL
//
//  Created by bytedance on 2020/12/23.
//


#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "initShader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>


// TODO: MatrixStack 类的定义
class MatrixStack {
    int    _index;
    int    _size;
    glm::mat4*  _matrices;

   public:
    MatrixStack( int numMatrices = 100 ):_index(0), _size(numMatrices)
        { _matrices = new glm::mat4[numMatrices]; }

    ~MatrixStack()
    { delete[]_matrices; }

    void push( const glm::mat4& m ) {
        assert( _index + 1 < _size );
        _matrices[_index++] = m;
         
    }

    glm::mat4& pop( void ) {
        assert( _index - 1 >= 0 );
        _index--;
        return _matrices[_index];
    }
};

MatrixStack  mvstack;
glm::mat4         model_view;
GLuint       ModelView, Projection;
GLuint       draw_color;


// TODO: define
#define TORSO_HEIGHT 5.0
#define TORSO_WIDTH 3.0
#define UPPER_ARM_HEIGHT 3.0
#define LOWER_ARM_HEIGHT 2.0
#define UPPER_LEG_WIDTH  0.5
#define LOWER_LEG_WIDTH  0.5
#define LOWER_LEG_HEIGHT 2.0
#define UPPER_LEG_HEIGHT 3.0
#define UPPER_LEG_WIDTH  0.5
#define UPPER_ARM_WIDTH  0.5
#define LOWER_ARM_WIDTH  0.5
#define HEAD_HEIGHT 2.0
#define HEAD_WIDTH 1.8

#define VERTICES_NUM 12
#define FACES_NUM 10

// TODO: enum
enum {
    Torso,
    Head1,
    Head2,
    RightUpperArm,
    RightLowerArm,
    LeftUpperArm,
    LeftLowerArm,
    RightUpperLeg,
    RightLowerLeg,
    LeftUpperLeg,
    LeftLowerLeg,
    NumJointAngles,
    Quit
};

// TODO: Person 类的定义
class Person {


    
    float vertices[VERTICES_NUM * 8] = {
    //  ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
        
    // 后面
     0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上 0
     0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 0.0f,   1.0f, 0.0f,   // 右下 1
    -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f,   // 左下 2
    -0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f,    // 左上 3
    
    // 前面
    0.5f,  0.5f, 0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上 4
    0.5f, -0.5f, 0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下 5
   -0.5f, -0.5f, 0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下 6
   -0.5f,  0.5f, 0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,    // 左上 7
        
    // 中间
    0.5f,  0.2f, -0.5f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f,   // 后右中 8
   -0.5f,  0.2f, -0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 后左中 9
    0.5f,  0.2f, 0.5f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f,   // 前右中 10
   -0.5f,  0.2f, 0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f   // 前左中 11
    };
    
    unsigned int indices[FACES_NUM * 6] = {
        0, 3, 9, 0, 8, 9, // 后面 - 上
        9, 2, 8, 8, 2, 1, // 后面 - 下
        0, 4, 8, 10, 4, 8, // 右面 - 上
        1, 8, 5, 10, 8, 5, // 右面 - 下
        0, 3, 4, 3, 4, 7, // 上面
        
        3, 9, 7, 11, 9, 7, // 左面 - 上
        2, 9, 11, 11, 6, 2, // 左面 - 下
        4, 10, 7, 7, 10, 11,  // 前面 - 上
        11, 5, 10, 11, 6, 5,  // 前面 - 下
        2, 1, 5, 2, 6, 5 // 下面

    };
    
    
public:
    Person() {}
    
    void init(unsigned int &personVAO, unsigned int &personVBO, unsigned int &personEBO){
        
        glGenVertexArrays(1, &personVAO);
        glGenBuffers(1, &personVBO);
        glGenBuffers(1, &personEBO);
        
        glBindVertexArray(personVAO);
        glBindBuffer(GL_ARRAY_BUFFER, personVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, personEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        
        // location = 0 -- position
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        
        // location = 1 -- color
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
//
//        // location = 1 -- color
//        glEnableVertexAttribArray(1);
//        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(3 * sizeof(float)));
    }
};

