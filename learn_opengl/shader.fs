#version 330 core
out vec4 FragColor;



in vec3 Normal;
in vec3 FragPos;
in vec3 LightPos;   // extra in variable, since we need the light position in view space we calculate this in the vertex shader//µÆÔ´µÄÎ»ÖÃ
in vec3 color;

void main()
{
	
	FragColor = vec4(color, 1.0);
}