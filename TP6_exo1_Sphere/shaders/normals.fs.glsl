#version 330 core

in vec3 viewPosition;
in vec3 viewNormal;
in vec2 outTexCoords;

out vec4 fragColor;

void main() {
    
    vec3 normalizedNormal = normalize(viewNormal);
    fragColor = vec4(normalizedNormal, 1.0);
}
