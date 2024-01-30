#include "p6/p6.h"
#include "glimac/default_shader.hpp"

int main()
{
    auto ctx = p6::Context{{1280, 720, "TP3 EX1"}};
    ctx.maximize_window();

    const p6::Shader shader = p6::load_shader(
    "shaders/grey2d.vs.glsl",
    "shaders/grey2d.fs.glsl"
    );

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo); 

    GLfloat vertices[] = {
     -0.5f, -0.5f, 1.f, 0.f, 0.f, 
     0.5f, -0.5f, 0.f, 1.f, 0.f, 
     0.0f,  0.5f, 0.f, 0.f, 1.f  
    };

    glBufferData(GL_ARRAY_BUFFER,15 * sizeof(GLfloat),vertices,GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    static constexpr GLuint vertex_attr_position = 0;
    static constexpr GLuint vertex_attr_color = 1;
    glEnableVertexAttribArray(vertex_attr_position);
    glEnableVertexAttribArray(vertex_attr_color);


    glBindBuffer(GL_ARRAY_BUFFER, vbo); 
    glVertexAttribPointer(vertex_attr_position, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (const GLvoid*)(0 * sizeof(GLfloat)));
    glVertexAttribPointer(vertex_attr_color, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat),(const GLvoid*)(2 * sizeof(GLfloat)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);




    // Declare your infinite update loop.
    ctx.update = [&]() {
        glClear(GL_COLOR_BUFFER_BIT);
        glBindVertexArray(vao);
        shader.use();
        glDrawArrays(GL_TRIANGLES,0,3);
        // glimac::bind_default_shader();
        glBindVertexArray(0);
        


    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}