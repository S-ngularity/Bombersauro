#version 130

uniform mat4 uModelMatrix;
uniform mat4 uProjMatrix;

in vec3 vColor;

void main(void) {
  gl_FragColor = vec4( vColor, 0.0);
}
