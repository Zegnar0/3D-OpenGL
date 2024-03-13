#version 330

in vec2 vTexCoords;
in vec3 vPosition_vs; // Position du sommet transformée dans l'espace View (vs)
in vec3 vNormal_vs; // Normale du sommet transformée dans l'espace View (vs)

out vec4 fFragColor;

uniform sampler2D uTexture;
uniform sampler2D ucloudTexture;

void main() {
    
    vec4 texture1 = texture(uTexture, vTexCoords);

    // Additionnez les deux couleurs des textures
    vec4 resultColor = texture1;

    // Vous pouvez également effectuer d'autres opérations ici si nécessaire

    fFragColor = resultColor;
}
