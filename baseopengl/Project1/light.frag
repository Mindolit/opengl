#version 330 core
out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{
	float ambinetStrength=0.1;
	vec3 ambient=ambinetStrength*lightColor;

	vec3 result= ambient*objectColor;


	FragColor=vec4(result,1.0);
}
