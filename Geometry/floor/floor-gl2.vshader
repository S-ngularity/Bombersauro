#version 110

uniform mat4 uModelMatrix;
uniform mat4 uProjMatrix;

attribute vec3 aPosition;
attribute vec3 aColor;

varying vec3 vColor;

void main() {
  vColor = aColor;
  gl_Position = uProjMatrix * uModelMatrix * vec4(aPosition, 1.0);
}
