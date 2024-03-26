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
        "shaders/pointLight.fs.glsl"
    );

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    const std::vector<glimac::ShapeVertex> vertices = glimac::sphere_vertices(1.f, 32, 16);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glimac::ShapeVertex), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

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

    GLuint uMVPMatrixLocation      = glGetUniformLocation(shader.id(), "uMVPMatrix");
    GLuint uMVMatrixLocation       = glGetUniformLocation(shader.id(), "uMVMatrix");
    GLuint uNormalMatrixLocation   = glGetUniformLocation(shader.id(), "uNormalMatrix");
    GLuint uKdLocation             = glGetUniformLocation(shader.id(), "uKd");
    GLuint uKsLocation             = glGetUniformLocation(shader.id(), "uKs");
    GLuint uShininessLocation      = glGetUniformLocation(shader.id(), "uShininess");
    GLuint uLightPos_vsLocation    = glGetUniformLocation(shader.id(), "uLightPos_vs");
    GLuint uLightIntensityLocation = glGetUniformLocation(shader.id(), "uLightIntensity");

    glEnable(GL_DEPTH_TEST);

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
        glm::mat4 ProjMatrix   = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);
        glm::mat4 MVMatrix     = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));
        MVMatrix               = glm::rotate(MVMatrix, ctx.time(), glm::vec3(0.f, 1.f, 0.f));
        glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

        glm::vec3 lightPos = glm::vec3(glm::rotate(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), ctx.time(), glm::vec3(0.0f, 1.0f, 0.0f)));
        lightPos *= 5.0f; // Scale the position for a larger spiral

        glm::vec3 lightPos_vs = glm::vec3(viewMatrix * glm::vec4(lightPos, 1.0f)); // Transform light position to view space

        glm::mat4 MVPMatrix = ProjMatrix * viewMatrix * MVMatrix;

        glUniformMatrix4fv(uMVPMatrixLocation, 1, GL_FALSE, glm::value_ptr(MVPMatrix));
        glUniformMatrix4fv(uMVMatrixLocation, 1, GL_FALSE, glm::value_ptr(MVMatrix));
        glUniformMatrix4fv(uNormalMatrixLocation, 1, GL_FALSE, glm::value_ptr(NormalMatrix));
        glUniform3f(uKdLocation, 0.6f, 0.2f, 0.3f);                         // Example values for diffuse reflectivity
        glUniform3f(uKsLocation, 0.8f, 0.8f, 0.8f);                         // Example values for specular reflectivity
        glUniform1f(uShininessLocation, 32.0f);                             // Example value for shininess
        glUniform3fv(uLightPos_vsLocation, 1, glm::value_ptr(lightPos_vs)); // Send transformed light position
        glUniform3f(uLightIntensityLocation, 1.0f, 1.0f, 1.0f);             // Example values for light intensity

        glDrawArrays(GL_TRIANGLES, 0, vertices.size());

        for (const auto& moonAxis : moonAxes)
        {
            // Generate random material values for each sphere
            glm::vec3 randomKd        = glm::linearRand(glm::vec3(0.0f), glm::vec3(1.0f));
            glm::vec3 randomKs        = glm::linearRand(glm::vec3(0.0f), glm::vec3(1.0f));
            float     randomShininess = glm::linearRand(1.0f, 128.0f);

            // Apply random material values
            glUniform3f(uKdLocation, randomKd.r, randomKd.g, randomKd.b);
            glUniform3f(uKsLocation, randomKs.r, randomKs.g, randomKs.b);
            glUniform1f(uShininessLocation, randomShininess);

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
        glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture unit GL_TEXTURE0
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture unit GL_TEXTURE1
        glBindVertexArray(0);
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    return EXIT_SUCCESS;
}
