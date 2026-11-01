#version 330 core
out vec4 frag_color;

in vec3 vertex_color;
in vec3 vertex_pos;
in vec2 texture_coord;

//uniform vec4 meow_color;
uniform sampler2D texture_object;

void main() {
   //frag_color = texture(texture_object, texture_coord) * vec4(vertex_color, 1.0);

   frag_color = texture(texture_object, texture_coord);
}