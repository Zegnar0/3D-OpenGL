#version 330 core

layout(location = 0) in vec2 aVertexPosition;
layout(location = 1) in vec2 aVertexUV;

out vec2 vFragUV;
out vec2 vVertexPosition;

uniform mat3 uModelMatrix;

void main() {
    vFragUV = aVertexUV;
    vVertexPosition = (uModelMatrix * vec3(aVertexPosition, 1.)).xy;

    gl_Position = vec4(vVertexPosition, 0.0, 2.0);
}
