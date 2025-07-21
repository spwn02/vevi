#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

int main()
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
    {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(800, 800, "Vevi", NULL, NULL);
    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
        std::cout << "Error" << std::endl;

    float positions[] = {
        -0.5f, -0.5f,
        0.5f, -0.5f,
        0.5f, 0.5f,
        -0.5f, 0.5f,
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0,
    };
    
    VertexArray va;
    VertexBuffer vb(positions, 2 * 4 * sizeof(float));

    VertexBufferLayout layout;
    layout.push<float>(2);
    va.addBuffer(vb, layout);

    IndexBuffer ib(indices, 6);

    Shader shader("../../resources/shaders/basic.shader");
    shader.bind();
    shader.setUniform4f("u_color", 0, 0, 0, 0);

    va.unbind();
    vb.unbind();
    ib.unbind();
    shader.unbind();

    Renderer renderer;

    struct RGB {
        float red;
        float green;
        float blue;
    };

    RGB rgb = { 1, 1, 1 };
    float incrementVal = 0.05f;
    float increment = incrementVal;
    char currentColor = 'r';

    float highThres = 1.0f;
    float lowThres = 0.3f;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        renderer.clear();

        shader.bind();
        shader.setUniform4f("u_color", rgb.red, rgb.green, rgb.blue, 1);

        renderer.draw(va, ib, shader);

        if (rgb.red >= highThres && rgb.blue >= highThres)
        {
            increment = -incrementVal;
            currentColor = 'r';
        }
        else if (rgb.red <= lowThres && rgb.blue <= lowThres)
        {
            increment = incrementVal;
            currentColor = 'r';
        }
        if (rgb.red <= lowThres && rgb.green >= highThres)
        {
            increment = -incrementVal;
            currentColor = 'g';
        }
        else if (rgb.red >= highThres && rgb.green <= lowThres)
        {
            increment = incrementVal;
            currentColor = 'g';
        }
        if (rgb.green <= lowThres && rgb.blue >= highThres)
        {
            increment = -incrementVal;
            currentColor = 'b';
        }
        else if (rgb.green >= highThres && rgb.blue <= lowThres)
        {
            increment = incrementVal;
            currentColor = 'b';
        }

        switch (currentColor)
        {
        case 'r':
            rgb.red += increment;
            break;
        case 'g':
            rgb.green += increment;
            break;
        case 'b':
            rgb.blue += increment;
            break;
        }

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
}