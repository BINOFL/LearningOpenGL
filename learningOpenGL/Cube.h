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

#define VERTICES_NUM 8
#define FACES_NUM 6

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
        
    // 前面
     0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
     0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
    -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
    -0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,    // 左上
    
    // 后面
    0.5f,  0.5f, 0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
    0.5f, -0.5f, 0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
   -0.5f, -0.5f, 0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
   -0.5f,  0.5f, 0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
        
    };
    
    unsigned int indices[FACES_NUM * 6] = {
        0, 1, 3, 1, 2, 3, // 前面
        0, 1, 5, 0, 4, 5, // 右面
        0, 3, 4, 3, 4, 7, // 上面
        2, 3, 7, 2, 6, 7, // 左面
        2, 1, 5, 2, 6, 5, // 下面
        4, 5, 6, 4, 6, 7  // 背面
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

