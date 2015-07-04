#version 150

uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uProjMatrix;

uniform vec3 uLightPosition;
uniform vec3 uLightIntensity;

in vec4 vVert;
in vec3 vNormal;
in vec3 vColor;

out vec4 out_Color;

void main()
{
	mat4 modelView = uViewMatrix * uModelMatrix;

	mat3 normalMatrix = transpose(inverse(mat3(modelView)));
	vec3 normal = normalize( normalMatrix * vNormal );

	vec3 viewPos = vec3( modelView * vVert );

	vec3 viewLightDirectional = vec3( uViewMatrix * vec4(uLightPosition, 0) );
	vec3 viewLightPoint = vec3( uViewMatrix * vec4(uLightPosition, 1) );
	
	vec3 vertexToLight = viewLightPoint - viewPos; 
	vec3 vertexToLightN = normalize(viewLightDirectional);

	float distance = length(vertexToLight);
	float attenuation = 1.0;// / (1 + 0.005 * distance * distance);

	// Ambient color
	float ambCoef = 0.05;
	vec3 ambient = ambCoef * uLightIntensity * vColor;

	// Difuse color
	float diffCoef = max(dot(vertexToLightN, normal), 0.0);
	vec3 diffuse = diffCoef * attenuation * vColor * uLightIntensity;

	// Specular color
	vec3 E = normalize(-viewPos); 
	vec3 R = normalize(-reflect(vertexToLightN, normal));

	float specCoef = pow(max(dot(R,E), 0.0), 100);
	vec3 specular = specCoef * attenuation * vColor*0.2 * uLightIntensity;

	out_Color = vec4(ambient + (1-ambCoef)*(diffuse + specular), 1.0);
}
