#version 330 core
out vec4 FragColor;
in vec3 outColor;
uniform vec4 unicolor;
void main()
{
  FragColor=vec4(outColor,1.0);
}