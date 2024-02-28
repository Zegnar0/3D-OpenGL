#include <cmath>
#include <cstddef>
#include <glm/gtc/type_ptr.hpp>
#include "glimac/common.hpp"
#include "glimac/cone_vertices.hpp"
#include "glimac/default_shader.hpp"
#include "glimac/sphere_vertices.hpp"
#include "p6/p6.h"

int main()
{
    auto ctx = p6::Context{{1280, 720, "TP3 EX1"}};
    ctx.maximize_window();

    const p6::Shader shader = p6::load_shader(
        "shaders/tex2D.vs.glsl",
        "shaders/normals.fs.glsl"
    );

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    const std::vector<glimac::ShapeVertex> vertices = glimac::sphere_vertices(1.f, 32, 16);

    glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(Vertex2DUV), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    GLuint triforceTexture;
    try
    {
        const std::string triforceImagePath = "assets/textures/triforce.png";
        img::Image        triforceImage     = p6::load_image_buffer(triforceImagePath); // Utilisez la fonction de votre bibliothèque

        glGenTextures(1, &triforceTexture);
        glBindTexture(GL_TEXTURE_2D, triforceTexture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, triforceImage.width(), triforceImage.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, triforceImage.data());
        glGenerateMipmap(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, 0); // Détachez la texture
    }
    catch (const std::exception& e)
    {
        std::cerr << "Erreur lors du chargement de la texture : " << e.what() << std::endl;
        // Gestion de l'erreur (terminer le programme, lever une exception, etc.)
        return EXIT_FAILURE;
    }

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

    GLint uColorLoc   = glGetUniformLocation(shader.id(), "uColor");
    GLint uTextureLoc = glGetUniformLocation(shader.id(), "uTexture");

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
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, triforceTexture);
        glUniform1i(uTextureLoc, 0);
        glUniformMatrix3fv(uModelMatrixLoc, 1, GL_FALSE, glm::value_ptr(translateTopLeft * rotateClockwise * initialScale * translateToOrigin * rotateClockwise * translateBackToPosition));
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUniformMatrix3fv(uModelMatrixLoc, 1, GL_FALSE, glm::value_ptr(translateTopRight * rotateAnticlockwise * initialScale * translateToOrigin * rotateAnticlockwise * translateBackToPosition));
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUniformMatrix3fv(uModelMatrixLoc, 1, GL_FALSE, glm::value_ptr(translateBottomLeft * rotateClockwise * initialScale * translateToOrigin * rotateClockwise * translateBackToPosition));
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUniformMatrix3fv(uModelMatrixLoc, 1, GL_FALSE, glm::value_ptr(translateBottomRight * rotateAnticlockwise * initialScale * translateToOrigin * rotateAnticlockwise * translateBackToPosition));
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindTexture(GL_TEXTURE_2D, 0);
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(0);
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    glDeleteTextures(1, &triforceTexture);

    return EXIT_SUCCESS;
}