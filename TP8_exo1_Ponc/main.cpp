#include <cmath>
#include <cstddef>
#include <glm/gtc/random.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glimac/FreeflyCamera.hpp"
#include "glimac/common.hpp"
#include "glimac/cone_vertices.hpp"
#include "glimac/default_shader.hpp"
#include "glimac/sphere_vertices.hpp"
#include "p6/p6.h"

int main()
{
    auto ctx = p6::Context{{1280, 720, "TP3 EX1"}};
    ctx.maximize_window();

    FreeflyCamera camera;

    const p6::Shader shader = p6::load_shader(
        "shaders/3D.vs.glsl",
        "shaders/tex3D.fs.glsl"
    );

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    const std::vector<glimac::ShapeVertex> vertices = glimac::sphere_vertices(1.f, 32, 16);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glimac::ShapeVertex), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    GLuint earthTexture;
    try
    {
        const std::string earthImagePath    = "assets/textures/EarthMap.jpg";
        img::Image        earthTextureImage = p6::load_image_buffer(earthImagePath); // Utilisez la fonction de votre bibliothèque

        glGenTextures(1, &earthTexture);
        glBindTexture(GL_TEXTURE_2D, earthTexture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, earthTextureImage.width(), earthTextureImage.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, earthTextureImage.data());
        glGenerateMipmap(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, 0);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Erreur lors du chargement de la texture : " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    GLuint MoonTexture;
    try
    {
        const std::string MoonImagePath    = "assets/textures/MoonMap.jpg";
        img::Image        MoonTextureImage = p6::load_image_buffer(MoonImagePath); // Utilisez la fonction de votre bibliothèque

        glGenTextures(1, &MoonTexture);
        glBindTexture(GL_TEXTURE_2D, MoonTexture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, MoonTextureImage.width(), MoonTextureImage.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, MoonTextureImage.data());
        glGenerateMipmap(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, 0);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Erreur lors du chargement de la texture : " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    static constexpr GLuint vertex_attr_position = 0;
    static constexpr GLuint vertex_attr_normal   = 1;
    static constexpr GLuint vertex_attr_UV       = 2;
    glEnableVertexAttribArray(vertex_attr_position);
    glEnableVertexAttribArray(vertex_attr_normal);
    glEnableVertexAttribArray(vertex_attr_UV);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(vertex_attr_position, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)offsetof(glimac::ShapeVertex, position));
    glVertexAttribPointer(vertex_attr_normal, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)offsetof(glimac::ShapeVertex, normal));
    glVertexAttribPointer(vertex_attr_UV, 2, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)offsetof(glimac::ShapeVertex, texCoords));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    GLuint uMVPMatrixLocation    = glGetUniformLocation(shader.id(), "uMVPMatrix");
    GLuint uMVMatrixLocation     = glGetUniformLocation(shader.id(), "uMVMatrix");
    GLuint uNormalMatrixLocation = glGetUniformLocation(shader.id(), "uNormalMatrix");
    // GLuint uTexture              = glGetUniformLocation(shader.id(), "uTexture");
    // GLint  uCloudTexture         = glGetUniformLocation(shader.id(), "uCloudTexture");
    glEnable(GL_DEPTH_TEST);
    // glUniform1i(uTextureSamplerLocation, 0);

    std::vector<glm::vec3> moonAxes;
    moonAxes.reserve(32);
    for (int i = 0; i < 32; ++i)
    {
        moonAxes.push_back(glm::sphericalRand(1.0f));
    }

    // Declare your infinite update loop.
    ctx.update = [&]() {
        glm::mat4 viewMatrix = camera.getViewMatrix();

        if (ctx.key_is_pressed(GLFW_KEY_D))
        {
            camera.moveLeft(-0.1f);
        };
        if (ctx.key_is_pressed(GLFW_KEY_A))
        {
            camera.moveLeft(0.1f);
        };
        if (ctx.key_is_pressed(GLFW_KEY_W))
        {
            camera.moveFront(0.1f);
        };
        if (ctx.key_is_pressed(GLFW_KEY_S))
        {
            camera.moveFront(-0.1f);
        };

        ctx.mouse_moved = [&camera](p6::MouseMove button) {
            camera.rotateLeft(-button.position.x);
            camera.rotateUp(button.position.y);
        };

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindVertexArray(vao);
        shader.use();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, earthTexture);
        glm::mat4 ProjMatrix   = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);
        glm::mat4 MVMatrix     = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));
        MVMatrix               = glm::rotate(MVMatrix, ctx.time(), glm::vec3(0.f, 1.f, 0.f));
        glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

        glm::mat4 MVPMatrix = ProjMatrix * viewMatrix * MVMatrix;

        glUniformMatrix4fv(uMVPMatrixLocation, 1, GL_FALSE, glm::value_ptr(MVPMatrix));
        glUniformMatrix4fv(uMVMatrixLocation, 1, GL_FALSE, glm::value_ptr(MVMatrix));
        glUniformMatrix4fv(uNormalMatrixLocation, 1, GL_FALSE, glm::value_ptr(NormalMatrix));
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());

        // MVMatrix = glm::rotate(MVMatrix, ctx.time(), {0.f, 1.f, 0.f});
        // MVMatrix = glm::translate(MVMatrix, {-2.f, 0.f, 0.f});
        // MVMatrix = glm::scale(MVMatrix, glm::vec3{0.2f});
        // glUniformMatrix4fv(uMVPMatrixLocation, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
        // glUniformMatrix4fv(uMVMatrixLocation, 1, GL_FALSE, glm::value_ptr(MVMatrix));
        // glUniformMatrix4fv(uNormalMatrixLocation, 1, GL_FALSE, glm::value_ptr(NormalMatrix));
        // glDrawArrays(GL_TRIANGLES, 0, vertices.size());

        glBindTexture(GL_TEXTURE_2D, MoonTexture);

        for (const auto& moonAxis : moonAxes)
        {
            MVMatrix = glm::rotate(MVMatrix, ctx.time() / 2, moonAxis);
            MVMatrix = glm::translate(MVMatrix, glm::vec3(-2.f, 0.f, 0.f));
            MVMatrix = glm::scale(MVMatrix, glm::vec3{0.2f});

            MVPMatrix = ProjMatrix * viewMatrix * MVMatrix;

            glUniformMatrix4fv(uMVPMatrixLocation, 1, GL_FALSE, glm::value_ptr(MVPMatrix));
            glUniformMatrix4fv(uMVMatrixLocation, 1, GL_FALSE, glm::value_ptr(MVMatrix));
            glUniformMatrix4fv(uNormalMatrixLocation, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(MVMatrix))));
            glDrawArrays(GL_TRIANGLES, 0, vertices.size());

            MVMatrix = glm::scale(MVMatrix, glm::vec3{5.f});
            MVMatrix = glm::translate(MVMatrix, glm::vec3(2.f, 0.f, 0.f));
        }
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0); // débind sur l'unité GL_TEXTURE0
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, 0); // débind sur l'unité GL_TEXTURE1
        glBindVertexArray(0);
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    return EXIT_SUCCESS;
}