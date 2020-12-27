// TODO: include 头文件合集
// TODO: 添加一行用来git测试的注释语句
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Model.h"

#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"
#include "texture.h"

//#include <learnopengl/shader_s.h>
#include "initShader.h"
#include "Persons.h"
#include "camera.h"


#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>



// TODO: 外部变量

#define WORLD_BOARD 50.0
#define FLOOR_BOARD 100.0

float planeVertices[] = {
    // positions          // texture Coords
     WORLD_BOARD, 0.0f,  WORLD_BOARD,  FLOOR_BOARD, 0.0f,
    -WORLD_BOARD, 0.0f,  WORLD_BOARD,  0.0f, 0.0f,
    -WORLD_BOARD, 0.0f, -WORLD_BOARD,  0.0f, FLOOR_BOARD,

     WORLD_BOARD, 0.0f,  WORLD_BOARD,  FLOOR_BOARD, 0.0f,
    -WORLD_BOARD, 0.0f, -WORLD_BOARD,  0.0f, FLOOR_BOARD,
     WORLD_BOARD, 0.0f, -WORLD_BOARD,  FLOOR_BOARD, FLOOR_BOARD
};

float skyboxVertices[] = {
    // positions
    -WORLD_BOARD,  WORLD_BOARD, -WORLD_BOARD,
    -WORLD_BOARD, -WORLD_BOARD, -WORLD_BOARD,
     WORLD_BOARD, -WORLD_BOARD, -WORLD_BOARD,
     WORLD_BOARD, -WORLD_BOARD, -WORLD_BOARD,
     WORLD_BOARD,  WORLD_BOARD, -WORLD_BOARD,
    -WORLD_BOARD,  WORLD_BOARD, -WORLD_BOARD,

    -WORLD_BOARD, -WORLD_BOARD,  WORLD_BOARD,
    -WORLD_BOARD, -WORLD_BOARD, -WORLD_BOARD,
    -WORLD_BOARD,  WORLD_BOARD, -WORLD_BOARD,
    -WORLD_BOARD,  WORLD_BOARD, -WORLD_BOARD,
    -WORLD_BOARD,  WORLD_BOARD,  WORLD_BOARD,
    -WORLD_BOARD, -WORLD_BOARD,  WORLD_BOARD,

     WORLD_BOARD, -WORLD_BOARD, -WORLD_BOARD,
     WORLD_BOARD, -WORLD_BOARD,  WORLD_BOARD,
     WORLD_BOARD,  WORLD_BOARD,  WORLD_BOARD,
     WORLD_BOARD,  WORLD_BOARD,  WORLD_BOARD,
     WORLD_BOARD,  WORLD_BOARD, -WORLD_BOARD,
     WORLD_BOARD, -WORLD_BOARD, -WORLD_BOARD,

    -WORLD_BOARD, -WORLD_BOARD,  WORLD_BOARD,
    -WORLD_BOARD,  WORLD_BOARD,  WORLD_BOARD,
     WORLD_BOARD,  WORLD_BOARD,  WORLD_BOARD,
     WORLD_BOARD,  WORLD_BOARD,  WORLD_BOARD,
     WORLD_BOARD, -WORLD_BOARD,  WORLD_BOARD,
    -WORLD_BOARD, -WORLD_BOARD,  WORLD_BOARD,

    -WORLD_BOARD,  WORLD_BOARD, -WORLD_BOARD,
     WORLD_BOARD,  WORLD_BOARD, -WORLD_BOARD,
     WORLD_BOARD,  WORLD_BOARD,  WORLD_BOARD,
     WORLD_BOARD,  WORLD_BOARD,  WORLD_BOARD,
    -WORLD_BOARD,  WORLD_BOARD,  WORLD_BOARD,
    -WORLD_BOARD,  WORLD_BOARD, -WORLD_BOARD,

    -WORLD_BOARD, -WORLD_BOARD, -WORLD_BOARD,
    -WORLD_BOARD, -WORLD_BOARD,  WORLD_BOARD,
     WORLD_BOARD, -WORLD_BOARD, -WORLD_BOARD,
     WORLD_BOARD, -WORLD_BOARD, -WORLD_BOARD,
    -WORLD_BOARD, -WORLD_BOARD,  WORLD_BOARD,
     WORLD_BOARD, -WORLD_BOARD,  WORLD_BOARD
};

// TODO: 定义：全局变量

Person person = Person();

const int SCR_WIDTH = 800;

const int SCR_HEIGHT = 600;

Camera camera = glm::vec3(0.0, 0.0, 0.0);
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

bool keys[1024];

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// TODO: 定义：回掉函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
GLuint loadCubemap(std::vector<const GLchar*> faces);
void do_movement();

// void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
// void mouse_callback(GLFWwindow* window, double xpos, double ypos);
// void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// TODO: main函数

int main(int argc, const char * argv[]) {

    memset(keys, false, 1024);
    
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation

    // --------------------

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);
    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
//    stbi_set_flip_vertically_on_load(true);
    
    glEnable(GL_DEPTH_TEST);
    
    // ------------------------------------------------------------
    //
    //
    // 结束初始化
    //
    //
    //
    // ------------------------------------------------------------
    
    // TODO: 设置 天空盒 的 VAO , VBO
    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glBindVertexArray(0);
    
    // Cubemap (Skybox)
    std::vector<const GLchar*> faces;
//    faces.push_back("skybox/right.jpg");
//    faces.push_back("skybox/left.jpg");
//    faces.push_back("skybox/top.jpg");
//    faces.push_back("skybox/down.jpg");
//    faces.push_back("skybox/back.jpg");
//    faces.push_back("skybox/front.jpg");
    
    faces.push_back("skybox-test/right-test.png");
    faces.push_back("skybox-test/left-test.png");
    faces.push_back("skybox-test/top-test.png");
    faces.push_back("skybox-test/bottom-test.png");
    faces.push_back("skybox-test/back-test.png");
    faces.push_back("skybox-test/front-test.png");
    
    unsigned cubemapTexture = loadCubemap(faces);
    
    // TODO: 设置 plane 的 VAO , VBO
    stbi_set_flip_vertically_on_load(true);
    unsigned int planeVAO, planeVBO;
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    
    
    // TODO: 设置 person 的 VAO , VBO, EBO
    // unsigned int personVAO, personVBO, personEBO;
    person.init();
    
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    
    // 　TODO: Shaders
    Shader skyboxShader = Shader("skybox.vert", "skybox.frag");
    Shader modelShader = Shader("shader.vert", "shader.frag");
    Shader planeShader = Shader("plane.vert", "plane.frag");
    Shader personShader = Shader("person.vert", "person.frag");
    person.setShader("person.vert", "person.frag");
//    Shader lightShader = Shader("light.vert", "shader.frag");
    
    
    
    // TODO: Models
    Model robotModel = Model("nanosuit/nanosuit.obj");
//    Model buildingModel1 = Model("Building_Obj/Building.obj");
//    Model buildingModel2 = Model("Building_Obj-2/Building.obj");
//    Model buildingModel3 = Model("Building_Obj-3/Building.obj");
    
    
    // TODO: Textures
//    unsigned int floorTexture = loadTexture("floor.jpeg");
    unsigned int floorTexture = loadTexture("test.png");
    unsigned int personTexture = loadTexture("metal.png");
    // glad: load all OpenGL function pointers

    // ---------------------------------------

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // TODO: while 循环
    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.2, 0.3, 0.3, 1.0);
        
        // ... 做点什么
        
        
        // TODO: 设置deltaTime时间
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        
        // TODO: 初始化MVP矩阵：
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        
        
        // TODO: 绘制地面：
        
//        // 设置是透明的
//        planeShader.use();
//
//        glEnable(GL_BLEND);
//        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
////        glDisable(GL_DEPTH_TEST);
////        glDepthMask(GL_FALSE);
//
//        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
//        planeShader.setMat4("model", model);
//        planeShader.setMat4("view", view);
//        planeShader.setMat4("projection", projection);
//        glBindVertexArray(planeVAO);
//        glBindTexture(GL_TEXTURE_2D, floorTexture);
//        glDrawArrays(GL_TRIANGLES, 0, 6);
//
//        glDisable(GL_BLEND);
//        glEnable(GL_DEPTH_TEST);
//        glDepthMask(GL_TRUE);
        // TODO: 绘制天空盒
        glDepthMask(GL_FALSE);// Remember to turn depth writing off
        skyboxShader.use();
        view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
        projection = glm::perspective(camera.Zoom, (float)SCR_WIDTH/(float)SCR_HEIGHT, 0.1f, 100.0f);
        skyboxShader.setMat4("model", model);
        skyboxShader.setMat4("view", view);
        skyboxShader.setMat4("projection", projection);
        // skybox cube
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glUniform1i(glGetUniformLocation(skyboxShader.ID, "skybox"), 0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthMask(GL_TRUE);
        

        
        // TODO: 绘制人物:
        model = glm::mat4(1.0f);
        view = camera.GetViewMatrix();
        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        personShader.use();

        person.display();
//          for(int i = 0; i < 2/*person.totalParts*/; i++) {
//              model = glm::mat4(1.0f);
            
//              model = glm::translate(model, person.translateArray[i]);
//              model = glm::scale(model, person.scaleArray[i]);
//  //            model = glm::translate(model, person.totalTranslate);
//              personShader.setMat4("model", model);
//              personShader.setMat4("view", view);
//              personShader.setMat4("projection", projection);
//              personShader.setVec4("u_Color", person.colorArray[i]);
//              glBindVertexArray(person.personVAO);
//      //        glDrawArrays(GL_TRIANGLES, 0, 32);
//              glBindTexture(GL_TEXTURE_2D, personTexture);
//              glDrawElements(GL_TRIANGLES, FACES_NUM * 6, GL_UNSIGNED_INT, 0);
//      //        ourModel.Draw(entityShader);
// //         }
// //        personShader.setMat4("model", glm::scale(model, glm::vec3(1.0, 1.0, 1.0)));
// //        personShader.setMat4("view", view);
// //        personShader.setMat4("projection", projection);
// //        glBindVertexArray(personVAO);
// ////        glDrawArrays(GL_TRIANGLES, 0, 32);
// //        glBindTexture(GL_TEXTURE_2D, personTexture);
// //        glDrawElements(GL_TRIANGLES, FACES_NUM * 6, GL_UNSIGNED_INT, 0);
// //        ourModel.Draw(entityShader);
        
        // 处理模型：
        modelShader.use();
        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        modelShader.setMat4("projection", projection);
        view = camera.GetViewMatrix();
        modelShader.setMat4("view", view);
        
        // robot
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0, 0.0, -5.0));
        model = glm::scale(model, glm::vec3(0.1, 0.1, 0.1));
        modelShader.setMat4("model", model);
        robotModel.Draw(modelShader);
        model = glm::mat4(1.0f);
        
        // building - 1
//        model = glm::translate(model, glm::vec3(0.0, -0.51, 0.0));
//        model = glm::scale(model, glm::vec3(0.001, 0.001, 0.001));
//        modelShader.setMat4("model", model);
//        buildingModel1.Draw(modelShader);
        
        // building - 2
        
        // 动一下：
        do_movement();
        
        // TODO: 渲染完毕
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glDeleteVertexArrays(1, &planeVAO);
    glDeleteVertexArrays(1, &person.personVAO);
    glDeleteBuffers(1, &planeVBO);
    glDeleteBuffers(1, &person.personVBO);
    
    glfwTerminate();
    return 0;
}

// TODO: 调用函数

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
//    std::cout << key << std::endl;
    if(action == GLFW_PRESS)
        keys[key] = true;
    else if(action == GLFW_RELEASE)
        keys[key] = false;
    if(keys[GLFW_KEY_ESCAPE]) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

void do_movement() {
    
    float speed = deltaTime * camera.MovementSpeed;
//    std::cout << "speed: " << speed << std::endl;
    if(keys[GLFW_KEY_A]) {
        camera.ProcessKeyboard(LEFT, speed);
    }
    if(keys[GLFW_KEY_D]) {
        camera.ProcessKeyboard(RIGHT, speed);
    }
    if(keys[GLFW_KEY_S] ) {
        camera.ProcessKeyboard(BACKWARD, speed);
    }
    if(keys [GLFW_KEY_W] ) {
        camera.ProcessKeyboard(FORWARD, speed);
    }
}

// TODO: 其他函数

// Loads a cubemap texture from 6 individual texture faces
// Order should be:
// +X (right)
// -X (left)
// +Y (top)
// -Y (bottom)
// +Z (front)
// -Z (back)
GLuint loadCubemap(std::vector<const GLchar*> faces)
{
    GLuint textureID;
    glGenTextures(1, &textureID);

    int width, height;
    unsigned char* image;
    int nrChannels;
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    for(unsigned int i = 0; i < faces.size(); i++)
    {
        image = stbi_load(faces[i], &width, &height, &nrChannels, STBI_rgb);
//        image = SOIL_load_image(faces[i], &width, &height, 0, SOIL_LOAD_RGB);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        stbi_image_free(image);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    return textureID;
}

