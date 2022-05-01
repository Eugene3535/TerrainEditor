#version 460 core

layout (location = 0) in vec3 point;   
  
out vec4 color; 

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform sampler2D texture1;

void main()
{
    color = texture(texture1, vec2(point.x, point.z)); 

    float height = 0.0f + ((color.r + color.g + color.b) / 3.0f) * 0.1f;

    gl_Position = projection * view * model * vec4(point.x, height, point.z, 1.0f);   
} 