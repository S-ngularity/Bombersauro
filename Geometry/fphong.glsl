#version 150

uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uProjMatrix;

uniform vec3 uLightPosition;
uniform vec3 uLightIntensity;

in vec3 vFragVert;
in vec3 vFragNormal;
in vec3 vColor;

out vec4 out_Color;

void main()
{
    mat3 normalMatrix = transpose(inverse(mat3(uModelMatrix)));
    vec3 normal = normalize(normalMatrix * vFragNormal);

    vec3 fragPosition = vec3(uModelMatrix * vec4(vFragVert, 1));

    vec3 surfaceToLight = uLightPosition - fragPosition;

    float brightness = dot(normal, surfaceToLight) / (length(surfaceToLight) * length(normal));
    //brightness = clamp(brightness, 0, 1);

    out_Color = vec4(brightness * uLightIntensity * vColor, 1.0);
}
