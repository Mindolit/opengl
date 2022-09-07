#version 330 core
out vec4 FragColor;
in vec3 outColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;

void main()
{
  FragColor=texture(ourTexture,TexCoord);
  //texture(유니폼 텍스쳐데이터,텍스쳐 좌표);
}