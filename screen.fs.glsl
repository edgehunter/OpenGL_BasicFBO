#version 130
uniform sampler2D tex;

in vec2 outtexcoord;

out vec4 frag_color;

void main () {
//frag_colour = vec4 (1.0, 0.0, 1.0, 0.0);
vec4 color =  vec4 (1.0, 0.0, 1.0, 0.0);
//frag_color = mix(color, texture(tex, outtexcoord), 0.7);
frag_color = texture(tex, outtexcoord);
}
