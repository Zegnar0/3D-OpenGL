#version 330 core

in vec2 vFragUV;  // Coordonnées de texture provenant du vertex shader

out vec3 fragColor;  // Couleur de sortie du fragment shader

uniform sampler2D uTexture;  // Variable uniforme pour la texture

void main() {
    // Utiliser la fonction texture pour lire la couleur de la texture aux coordonnées vFragUV
    vec4 textureColor = texture(uTexture, vFragUV);

    // Utiliser la couleur lue depuis la texture comme couleur de sortie
    fragColor = textureColor.rgb;  // Convertir vec4 en vec3
}