#version 130
uniform sampler2D tex;

in vec2 texcoord;

out vec4 frag_colour;
void main () {
//frag_colour = vec4 (1.0, 0.0, 1.0, 0.0);
vec4 color =  vec4 (1.0, 0.0, 1.0, 0.0);
frag_color = mix(color, texture(tex, texcoord), 0.7); 
}
