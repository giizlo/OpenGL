#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <ctime>
#include <iostream>
#include <fstream>
#include <sstream>


#include "Buffers.h"


#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_CAPTION "TITLE"


std::string readFile(const char* filename)
{
    std::string text;
	std::ifstream file(filename, std::ios::in);

    if (file.is_open()) 
    { 
		std::stringstream sstr; 
		sstr << file.rdbuf(); 
		text = sstr.str();
		file.close();
	}

    return text;
}


GLuint LoadShaders(const char *vertex_file, const char *fragment_file)
{
    GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    GLint result = GL_FALSE;
    int infoLogLength;

    
    std::string code = readFile(vertex_file);
    const char* pointer = code.c_str(); 


    glShaderSource(vertexShaderID, 1, &pointer, NULL);
    glCompileShader(vertexShaderID);

    glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 0)
    {
        char* errorMessage = new char[infoLogLength + 1];
        glGetShaderInfoLog(vertexShaderID, infoLogLength, NULL, errorMessage);
        std::cout << errorMessage;
        delete[] errorMessage;
    }
    

    code = readFile(fragment_file);
    pointer = code.c_str();


    glShaderSource(fragmentShaderID, 1, &pointer, NULL);
    glCompileShader(fragmentShaderID);

    glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 0)
    {
        char* errorMessage = new char[infoLogLength + 1];
        glGetShaderInfoLog(fragmentShaderID, infoLogLength, NULL, errorMessage);
        std::cout << errorMessage;
        delete[] errorMessage;
    }


    GLuint programID = glCreateProgram();
    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);
    glLinkProgram(programID);


    glGetProgramiv(programID, GL_LINK_STATUS, &result);
    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 0)
    {
        char* errorMessage = new char[infoLogLength + 1];
        glGetProgramInfoLog(programID, infoLogLength, NULL, errorMessage);
        std::cout << errorMessage;
        delete[] errorMessage;
    }


    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);

    return programID;
}



void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

int main(void)
{
    if (!glfwInit())
    {
        std::cout << "GLFW init error\n";
        return -1;
    }

    atexit(glfwTerminate);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_CAPTION, NULL, NULL);
    if (!window)
    {
        std::cout << "GLFW create window error\n";
       return -1;
    }
    
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "GLAD load GL error\n";
        return -1; 
    }


    GLuint clockProgram = LoadShaders("../shaders/clock.vert", "../shaders/clock.frag");
    GLuint shapeProgram = LoadShaders("../shaders/shape.vert", "../shaders/shape.frag");
    
    
    float screenVertices[] = {
        // positions   // color
        -1.0f,  1.0f,  0.5f, 0.0f, 0.2f,
        -1.0f, -1.0f,  0.5f, 0.0f, 0.2f,
         1.0f, -1.0f,  0.5f, 0.0f, 0.2f,

        -1.0f,  1.0f,  0.5f, 0.0f, 0.2f,
         1.0f, -1.0f,  0.5f, 0.0f, 0.2f,
         1.0f,  1.0f,  0.5f, 0.0f, 0.2f
    };


    VAO bgVAO;
    BO bgVBO(VERTEX);
    bgVBO.load(screenVertices, sizeof(screenVertices));
    glVertexAttribPointer(
            0, // индекс атрибута 
            2, // количество компонент одного элемента
            GL_FLOAT, // тип
            GL_FALSE, // необходимость нормировать значения
            5 * sizeof(float), // шаг
            (void*)0 // отступ с начала массива
    );
    glEnableVertexAttribArray(0); // индекс location, по которому данные передадутся в layout шейдера
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);



    float clockVertices[] = {
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };
    
    VAO clockVAO;
    BO clockVBO(VERTEX);
    clockVBO.load(clockVertices, sizeof(clockVertices));
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    

    
    float triangleVertices[] = {
        // positions   // colors
         0.0f,  0.5f,  1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
         0.5f, -0.5f,  0.0f, 0.0f, 1.0f
    };

    VAO triangleVAO;
    BO triangleVBO(VERTEX);
    triangleVBO.load(triangleVertices, sizeof(triangleVertices));
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);


    GLint projectionLoc_c = glGetUniformLocation(clockProgram, "projection"); 
    GLint projectionLoc_s = glGetUniformLocation(shapeProgram, "projection"); 

    glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f); // Создаем ортографическую проекцию

    GLint resolutionLoc_c = glGetUniformLocation(clockProgram, "resolution");
    GLint timeLoc_c = glGetUniformLocation(clockProgram, "time"); 
    GLint timeNow_c = glGetUniformLocation(clockProgram, "now");
    
    
    float dx  = 0.0, dy = 0.0;

    glClearColor(0.5f, 0.0f, 0.2f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    while(!glfwWindowShouldClose(window))
    {
        
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shapeProgram);
        glUniformMatrix4fv(projectionLoc_s, 1, GL_FALSE, &projection[0][0]); 

        bgVAO.use();
        glDrawArrays(GL_TRIANGLES, 0, 6);


        float timeValue = glfwGetTime();
        float moveX = sin(timeValue) * 0.5f;
        float moveY = cos(timeValue) * 0.5f;
        
        float updatedTriangleVertices[] = {
            // positions           // colors
            moveX,      0.5f + moveY,  1.0f, 0.0f, 0.0f,
            -0.5f + moveX, -0.5f + moveY,  0.0f, 1.0f, 0.0f,
             0.5f + moveX, -0.5f + moveY,  0.0f, 0.0f, 1.0f
        };

        triangleVBO.load(updatedTriangleVertices, sizeof(updatedTriangleVertices));

        glUseProgram(shapeProgram);

        glUniformMatrix4fv(projectionLoc_s, 1, GL_FALSE, &projection[0][0]); 

        triangleVAO.use();
        glDrawArrays(GL_TRIANGLES, 0, 3);


       
        dy += glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS ?  0.001 : 0.0;
        dy -= glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS ?  0.001 : 0.0;
        dx -= glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS ?  0.001 : 0.0;
        dx += glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS ?  0.001 : 0.0;


        float updatedClockVertices[] = {
            // positions             // texCoords
            -1.0f + dx,  1.0f + dy,  0.0f, 1.0f,  // Верхний левый угол
            -1.0f + dx, -1.0f + dy,  0.0f, 0.0f,  // Нижний левый угол
             1.0f + dx, -1.0f + dy,  1.0f, 0.0f,  // Нижний правый угол
    
            -1.0f + dx,  1.0f + dy,  0.0f, 1.0f,  // Верхний левый угол (повтор)
             1.0f + dx, -1.0f + dy,  1.0f, 0.0f,  // Нижний правый угол (повтор)
             1.0f + dx,  1.0f + dy,  1.0f, 1.0f   // Верхний правый угол
        };

        clockVBO.load(updatedClockVertices, sizeof(updatedClockVertices));
        
        glUseProgram(clockProgram);


        glUniform2f(resolutionLoc_c, WINDOW_WIDTH, WINDOW_HEIGHT);
            time_t t1 = time(NULL);
            tm t = *localtime(&t1);
        glUniform1f(timeLoc_c, t.tm_sec + (t.tm_hour*3600) + (t.tm_min*60));
        glUniform1f(timeNow_c, (float)glfwGetTime());

        glUniform1f(timeNow_c, (float)glfwGetTime());

        glUniformMatrix4fv(projectionLoc_c, 1, GL_FALSE, &projection[0][0]); 

        clockVAO.use();
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glDeleteProgram(clockProgram);
    glDeleteProgram(shapeProgram);
    glfwTerminate();
    
    return 0;
}
