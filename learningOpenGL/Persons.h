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
glm::mat4    model_view;
GLuint       ModelView, Projection;
GLuint       draw_color;


// TODO: define
#define TORSO_HEIGHT 4.0
#define TORSO_WIDTH 3.0

#define UPPER_ARM_HEIGHT 2.5
#define UPPER_ARM_WIDTH  2.0

#define LOWER_ARM_HEIGHT 2.0
#define LOWER_ARM_WIDTH  2.0

#define UPPER_LEG_HEIGHT 3.0
#define UPPER_LEG_WIDTH  2.5

#define LOWER_LEG_HEIGHT 2.0
#define LOWER_LEG_WIDTH  2.0

#define HEAD_HEIGHT 2.0
#define HEAD_WIDTH 1.8

//#define TORSO_HEIGHT 0.5
//#define TORSO_WIDTH 0.3
//#define UPPER_ARM_HEIGHT 0.3
//#define LOWER_ARM_HEIGHT 0.2
//#define UPPER_LEG_WIDTH  0.05
//#define LOWER_LEG_WIDTH  0.05
//#define LOWER_LEG_HEIGHT 0.2
//#define UPPER_LEG_HEIGHT 0.3
//#define UPPER_LEG_WIDTH  0.05
//#define UPPER_ARM_WIDTH  0.05
//#define LOWER_ARM_WIDTH  0.05
//#define HEAD_HEIGHT 0.2
//#define HEAD_WIDTH 0.18

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

GLfloat
theta[NumJointAngles] = {
    0.0,    // Torso
    0.0,    // Head1
    0.0,    // Head2
    0.0,    // RightUpperArm
    0.0,    // RightLowerArm
    0.0,    // LeftUpperArm
    0.0,    // LeftLowerArm
    180.0,  // RightUpperLeg
    0.0,    // RightLowerLeg
    180.0,  // LeftUpperLeg
    0.0     // LeftLowerLeg
};
// TODO: Person 类的定义
class Person {

public:
    unsigned int personVAO, personVBO, personEBO;
private:

    // TODO: private 数据
    float vertices[VERTICES_NUM * 8] = {
    //  ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
    // 前面

    -0.5, -0.5,  0.5,    1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
    -0.5,  0.5,  0.5,    0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
     0.5,  0.5,  0.5,    0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
     0.5, -0.5,  0.5,    1.0f, 1.0f, 0.0f,   0.0f, 1.0f,    // 左上
    
    // 后面
    -0.5, -0.5, -0.5,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
    -0.5,  0.5, -0.5,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
     0.5,  0.5, -0.5,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
     0.5, -0.5, -0.5,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
        
    };
    
    unsigned int indices[FACES_NUM * 6] = {
        0, 1, 3, 1, 2, 3, // 前面
        0, 1, 5, 0, 4, 5, // 右面
        0, 3, 4, 3, 4, 7, // 上面
        2, 3, 7, 2, 6, 7, // 左面
        2, 1, 5, 2, 6, 5, // 下面
        4, 5, 6, 4, 6, 7  // 背面
    };

    const static int numVertices = 36;

    const static unsigned int totalParts = 10;
    
    glm::vec3 totalTranslate = glm::vec3(0.0, 0.0, -3.0);
    
    glm::vec4 colorArray[totalParts] = {
        glm::vec4( 0.0, 0.0, 0.0, 1.0 ),  // black
        glm::vec4( 1.0, 0.0, 0.0, 1.0 ),  // red
        glm::vec4( 1.0, 1.0, 0.0, 1.0 ),  // yellow
        glm::vec4( 0.0, 1.0, 0.0, 1.0 ),  // green
        glm::vec4( 0.0, 0.0, 1.0, 1.0 ),  // blue
        glm::vec4( 1.0, 0.0, 1.0, 1.0 ),  // magenta
        glm::vec4( 1.0, 1.0, 1.0, 1.0 ),  // white
        glm::vec4( 0.0, 1.0, 1.0, 1.0 )   // cyan
    };
    
    glm::vec3 scaleArray[totalParts] = {
        glm::vec3(TORSO_WIDTH, TORSO_HEIGHT, TORSO_WIDTH), // 身体
//        glm::vec3(1.0, 1.0, 1.0),
//        glm::vec3(1.0, 1.0, 1.0),
        glm::vec3(UPPER_ARM_WIDTH, UPPER_ARM_HEIGHT, UPPER_ARM_WIDTH), // 左上臂
        glm::vec3(LOWER_ARM_WIDTH, LOWER_ARM_HEIGHT, LOWER_ARM_WIDTH), // 左下臂

        glm::vec3(UPPER_ARM_WIDTH, UPPER_ARM_HEIGHT, TORSO_WIDTH), // 右上臂
        glm::vec3(LOWER_ARM_WIDTH, LOWER_ARM_HEIGHT, TORSO_WIDTH), // 右下臂

        glm::vec3(UPPER_LEG_WIDTH, UPPER_LEG_HEIGHT, UPPER_LEG_WIDTH), // 左上腿
        glm::vec3(LOWER_LEG_WIDTH, LOWER_LEG_HEIGHT, LOWER_LEG_WIDTH), // 左下腿

        glm::vec3(UPPER_LEG_WIDTH, UPPER_LEG_HEIGHT, UPPER_LEG_WIDTH), // 右上腿
        glm::vec3(LOWER_LEG_WIDTH, LOWER_LEG_HEIGHT, LOWER_LEG_WIDTH),  // 右下腿
    
//        glm::vec3(HEAD_WIDTH, HEAD_HEIGHT, TORSO_WIDTH) // 头
    };
    
    glm::vec3 translateArray[totalParts] = {
        glm::vec3(0.0, 0.0, 0.0), //身体
//        glm::vec3(0.0, 1.0, 1.0),
//        glm::vec3(1.0, 1.0, 1.0),
        glm::vec3(UPPER_ARM_WIDTH/2 + TORSO_WIDTH/2, 0.0, 0.0), // 左上臂
        glm::vec3(0.0, -UPPER_ARM_HEIGHT, 0.0), // 左下臂

        glm::vec3(TORSO_WIDTH/2.0 + UPPER_ARM_WIDTH, TORSO_HEIGHT, 0.0), // 右上臂
        glm::vec3(0.0, -UPPER_ARM_HEIGHT, 0.0), // 右下臂

        glm::vec3(-TORSO_WIDTH, -UPPER_LEG_HEIGHT, 0.0), // 左上腿
        glm::vec3(0.0, -LOWER_LEG_HEIGHT, 0.0), // 左上腿

        glm::vec3(TORSO_WIDTH, -UPPER_LEG_HEIGHT, 0.0), // 右上腿
        glm::vec3(0.0, -LOWER_LEG_HEIGHT, 0.0), // 右上腿

//        glm::vec3(0.0, TORSO_HEIGHT + HEAD_HEIGHT, 0.0) // 头
    };

    // TODO: private函数
    void torso(){
        mvstack.push(model_view);

        glm::mat4 instance = glm::translate(model_view, translateArray[Torso]);
        instance = glm::scale(instance, scaleArray[Torso]);
        shader.setMat4("model_view", model_view * instance);
        shader.setMat4("projection", glm::mat4(1.0f));
        shader.setVec4("u_Color", colorArray[Torso]);
        glBindVertexArray(personVAO);
//        glBindTexture(GL_TEXTURE_2D, )
        glDrawElements(GL_TRIANGLES, FACES_NUM * 6, GL_UNSIGNED_INT, 0);
//        glDrawArrays(GL_TRIANGLES, 0, numVertices);
        model_view = mvstack.pop();
    }

public:
    
    Shader shader;

    
    Person() {
    }
    
    void setShader(const char* vertexPath, const char* fragmentPath) {
        shader = Shader(vertexPath, fragmentPath);
    }
    
    void init(){
        
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
    
    // TODO: 绘制函数
    void display() {

        shader.use();

        // 围绕y轴旋转
        model_view = glm::rotate(glm::mat4(1.0f), 
                                glm::radians(theta[Torso]), 
                                glm::vec3(0.0, 1.0, 0.0));
        torso();

    }
};

