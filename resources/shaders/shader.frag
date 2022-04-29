#version 460 core

out vec4 FragColor;  
in vec2 texcoord;

uniform sampler2D ourTexture;
  
void main()
{
    FragColor = texture(ourTexture, texcoord);
}
