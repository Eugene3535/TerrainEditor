#version 460 core

out vec4 FragColor;  
in vec2 texcoord;

uniform sampler2D texture1;
  
void main()
{
    FragColor = texture(texture1, texcoord);
}
