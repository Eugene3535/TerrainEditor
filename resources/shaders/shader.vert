#version 460 core

layout (location = 0) in vec3 position;  
layout (location = 1) in vec2 in_tex_coords;  
  
out vec2 tex_coords; 

uniform mat4 model = mat4(1);
uniform mat4 view = mat4(1);
uniform mat4 projection = mat4(1);

void main()
{
    //float height = 0.0f - ((color.r + color.g + color.b) * 0.3f) * 0.1f;
    tex_coords = in_tex_coords;
    gl_Position = projection * view * model * vec4(position, 1.0f);   
} 