#version 330 core
out vec4 FragColor;
in vec3 outColor;
in vec2 TexCoord;

uniform sampler2D pepeTexture;
uniform sampler2D wallTexture;

void main()
{
	FragColor=mix(texture(pepeTexture,TexCoord),texture(wallTexture,TexCoord),0.5);	
  //texture(유니폼 텍스쳐데이터,텍스쳐 좌표);
}