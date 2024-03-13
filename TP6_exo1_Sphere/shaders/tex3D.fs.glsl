#version 330

in vec2 vTexCoords;
in vec3 vPosition_vs; // Position du sommet transformée dans l'espace View (vs)
in vec3 vNormal_vs; // Normale du sommet transformée dans l'espace View (vs)

out vec4 fFragColor;

uniform sampler2D uTexture;

void main() {

    vec4 texture1 = texture(uTexture, vTexCoords);
    //vec3 normalizedNormal = normalize(vNormal_vs);
    //fFragColor = vec4(normalizedNormal, 1.);
    fFragColor = texture1;
}