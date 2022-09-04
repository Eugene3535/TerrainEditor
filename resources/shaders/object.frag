#version 460 core

in vec2 tex_coords;

uniform sampler2D texture0;

out vec4 FragColor;  
  
void main()
{
    vec4 color = texture(texture0, tex_coords);

    if(color.a < 0.01f)
        discard;

    FragColor = color;
}
