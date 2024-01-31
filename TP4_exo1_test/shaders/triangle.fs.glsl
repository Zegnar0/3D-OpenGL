#version 330 core

in vec3 vFragColor;
in vec2 vVertexPosition;

out vec4 fFragColor;

float alpha = 20; 
float b = 40; 
float a = alpha*exp(-b*pow(distance(vec2(0.0, 0.0), vVertexPosition),2));

void main() {
  fFragColor = vec4(vFragColor*a, 1.);
}