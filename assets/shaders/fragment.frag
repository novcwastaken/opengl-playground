#version 330 core
out vec4 frag_color;
in vec3 vertex_color;

//uniform vec4 meow_color;

void main() {
   //frag_color = vec4(72.0f/255, 167.0f/255, 240.0f/255, 1.0f);
   frag_color = vec4(vertex_color, 1.0f);
   //frag_color = meow_color;
}