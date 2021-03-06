#version 150

uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uProjMatrix;

uniform vec3 uLightPosition;
uniform vec3 uLightIntensity;

in vec3 aPosition;
in vec3 aColor;
in vec3 aNormal;

out vec4 vVert;
out vec3 vNormal;
out vec3 vColor;

void main()
{
	vVert = vec4(aPosition, 1.0);
	vNormal = aNormal;
	vColor = aColor;

	gl_Position = uProjMatrix * uViewMatrix * uModelMatrix * vec4(aPosition, 1.0);

}
