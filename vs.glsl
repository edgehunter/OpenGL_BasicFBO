#version 130
in vec3 vp;
in vec2 texcoord;

out vec2 outtexcoord;

void main () {
//gl_PointSize = 20.0f;
gl_Position = vec4 (vp, 1.0);
outtexcoord = texcoord; 
}
