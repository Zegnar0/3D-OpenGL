#version 330

in vec2 vTexCoords;
in vec3 vPosition_vs; // Position du sommet transformée dans l'espace View (vs)
in vec3 vNormal_vs; // Normale du sommet transformée dans l'espace View (vs)

out vec4 fFragColor;

uniform vec3 uKd;
uniform vec3 uKs;
uniform float uShininess;
uniform vec3 uLightDir_vs;
uniform vec3 uLightIntensity;

vec3 blinnPhong(){
    vec3 N = normalize(vNormal_vs);
    vec3 L = normalize(uLightDir_vs);
    vec3 V = normalize(-vPosition_vs);
    vec3 H = normalize(L + V);
    float dotNL = max(0.0, dot(N, L));
    float dotNH = max(0.0, dot(N, H));
    vec3 diffuse = uKd * dotNL;
    vec3 specular = uKs * pow(dotNH, uShininess);
    return uLightIntensity * (diffuse + specular);
}

void main() {


    fFragColor = vec4(blinnPhong(), 1.0);
}
