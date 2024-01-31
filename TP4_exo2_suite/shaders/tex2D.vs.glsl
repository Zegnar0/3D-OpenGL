#version 330 core

layout(location = 0) in vec2 aVertexPosition;
layout(location = 1) in vec2 aVertexUV;

out vec2 vFragUV;
out vec2 vVertexPosition;

void main() {
    //red color
    vFragUV = aVertexUV;
    vVertexPosition = aVertexPosition;
    gl_Position = vec4(aVertexPosition, 0.0, 1.0);
    
    
}