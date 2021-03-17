#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPos;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 viewPos;

out vec3 Normal;
out vec3 FragPos;  
out vec3 LightPos;
out vec3 color;

void main()
{
   gl_Position = projection * view * model * vec4(aPos, 1.0);
   FragPos = vec3(view * model * vec4(aPos, 1.0));//顶点在世界空间中的位置
   Normal = mat3(transpose(inverse(view * model))) * aNormal;//顶点的法向量
   LightPos = vec3(view * vec4(lightPos, 1.0));// Transform world-space light position to view-space light position

   //环境光照
	float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
	//漫反射光
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(LightPos - FragPos);

	float diff = max(dot(norm, lightDir), 0.0);//如果两个向量之间的角度大于90度，点乘的结果就会变成负数，这样会导致漫反射分量变为负数。
	vec3 diffuse = diff * lightColor;
	//镜面光照
	float specularStrength = 2;//镜面强度(Specular Intensity)
	vec3 viewDir = normalize(-FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);//reflect函数要求第一个向量是从光源指向片段位置的向量，所以取反
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);//32是高光的反光度(Shininess)，一个物体的反光度越高，反射光的能力越强，散射得越少，高光点就会越小。
	vec3 specular = specularStrength * spec * lightColor;


	color = (ambient + diffuse + specular) * objectColor;

};