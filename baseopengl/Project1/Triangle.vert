#version 330 core
layout(location=0) in vec3 aPos;
layout(location=1) in vec3 aColor;

out vec3 outColor;

uniform vec4 horizontal_offset;
void main()
{
	gl_Position=vec4(aPos.x+horizontal_offset.x,aPos.y+horizontal_offset.y,aPos.z+horizontal_offset.z,1.0);
	outColor=aColor;
}