#version 330

in vec2 vTexCoords;
in vec3 vPosition_vs; // Position du sommet transformée dans l'espace View (vs)
in vec3 vNormal_vs; // Normale du sommet transformée dans l'espace View (vs)

out vec4 fFragColor;

uniform vec3 uKd;
uniform vec3 uKs;
uniform float uShininess;
uniform vec3 uLightPos_vs;
uniform vec3 uLightIntensity;

vec3 blinnPhong(){
    // normalisation des vecteurs
    vec3 N = normalize(vNormal_vs);
    vec3 L = normalize(uLightPos_vs - vPosition_vs);
    vec3 V = normalize(-vPosition_vs);
    vec3 H = normalize(L + V);

    // calcul des facteurs
    float NdotL = max(0.0, dot(N, L));
    float NdotH = max(0.0, dot(N, H));

    // calcul de l'éclairement spéculaire
    vec3 color = uKd * NdotL * uLightIntensity;

    // calcul de l'éclairement diffus
    color += uKs * pow(NdotH, uShininess) * uLightIntensity;

    return color;
}

void main() {


    fFragColor = vec4(blinnPhong(), 1.0);
}
