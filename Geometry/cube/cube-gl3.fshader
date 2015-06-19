#version 130

uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uProjMatrix;

in vec3 vColor;

out vec4 out_Color;

void main(void) {
  out_Color = vec4( vColor, 0.0);
}
