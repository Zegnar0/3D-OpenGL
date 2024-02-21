#include <cmath>
#include <cstddef>
#include <glm/gtc/type_ptr.hpp>
#include "glimac/default_shader.hpp"
#include "p6/p6.h"

struct Vertex2DUV {
    glm::vec2 position;
    glm::vec2 texture_uv;
};

glm::mat3 translate(float tx, float ty)
{
    return glm::mat3(
        glm::vec3(1, 0, 0),
        glm::vec3(0, 1, 0),
        glm::vec3(tx, ty, 1)
    );
}

glm::mat3 scale(float sx, float sy)
{
    return glm::mat3(
        glm::vec3(sx, 0, 0),
        glm::vec3(0, sy, 0),
        glm::vec3(0, 0, 1)
    );
}

glm::mat3 rotate(float a)
{
    return glm::mat3(
        glm::vec3(std::cos(a), std::sin(a), 0),
        glm::vec3(-std::sin(a), std::cos(a), 0),
        glm::vec3(0, 0, 1)
    );
}

int main()
{
    auto ctx = p6::Context{{1280, 720, "TP3 EX1"}};
    ctx.maximize_window();

    const p6::Shader shader = p6::load_shader(
        "shaders/tex2D.vs.glsl",
        "shaders/tex2D.fs.glsl"
    );

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    Vertex2DUV vertices[] = {
        {glm::vec2(-1.f, -1.f), glm::vec2(0.f, 0.f)},
        {glm::vec2(1.f, -1.f), glm::vec2(0.f, 0.f)},
        {glm::vec2(0.0f, 1.f), glm::vec2(0.f, 0.f)}
    };

    glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(Vertex2DUV), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    static constexpr GLuint vertex_attr_position = 0;
    static constexpr GLuint vertex_attr_UV       = 1;
    glEnableVertexAttribArray(vertex_attr_position);
    glEnableVertexAttribArray(vertex_attr_UV);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(vertex_attr_position, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2DUV), (const GLvoid*)offsetof(Vertex2DUV, position));
    glVertexAttribPointer(vertex_attr_UV, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2DUV), (const GLvoid*)offsetof(Vertex2DUV, texture_uv));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    GLint uColorLoc = glGetUniformLocation(shader.id(), "uColor");

    // Declare your infinite update loop.
    ctx.update = [&]() {
        glClear(GL_COLOR_BUFFER_BIT);
        glBindVertexArray(vao);
        shader.use();

        glm::mat3 initialScale = scale(0.25f, 0.25f);

        float     clockwiseRotation     = ctx.time();
        float     anticlockwiseRotation = -ctx.time();
        glm::mat3 rotateClockwise       = rotate(clockwiseRotation);
        glm::mat3 rotateAnticlockwise   = rotate(anticlockwiseRotation);

        glm::mat3 translateTopLeft     = translate(-0.5f, 0.5f);
        glm::mat3 translateTopRight    = translate(0.5f, 0.5f);
        glm::mat3 translateBottomLeft  = translate(-0.5f, -0.5f);
        glm::mat3 translateBottomRight = translate(0.5f, -0.5f);

        glm::mat3 translateToOrigin       = translate(0.5f, 0.5f);
        glm::mat3 translateBackToPosition = translate(-0.5f, -0.5f);
        GLint     uModelMatrixLoc         = glGetUniformLocation(shader.id(), "uModelMatrix");
        glUniformMatrix3fv(uModelMatrixLoc, 1, GL_FALSE, glm::value_ptr(translateTopLeft * rotateClockwise * initialScale * translateToOrigin * rotateClockwise * translateBackToPosition));
        glUniform3f(uColorLoc, 1.0f, 0.25f, 0.f);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUniformMatrix3fv(uModelMatrixLoc, 1, GL_FALSE, glm::value_ptr(translateTopRight * rotateAnticlockwise * initialScale * translateToOrigin * rotateAnticlockwise * translateBackToPosition));
        glUniform3f(uColorLoc, 1.0f, 0.0f, 0.0f);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUniformMatrix3fv(uModelMatrixLoc, 1, GL_FALSE, glm::value_ptr(translateBottomLeft * rotateClockwise * initialScale * translateToOrigin * rotateClockwise * translateBackToPosition));
        glUniform3f(uColorLoc, 1.0f, 1.0f, 0.0f);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUniformMatrix3fv(uModelMatrixLoc, 1, GL_FALSE, glm::value_ptr(translateBottomRight * rotateAnticlockwise * initialScale * translateToOrigin * rotateAnticlockwise * translateBackToPosition));
        glUniform3f(uColorLoc, 0.0f, 0.0f, 1.0f);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}