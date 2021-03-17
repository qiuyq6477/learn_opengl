#version 330 core
out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 viewPos;

in vec3 Normal;
in vec3 FragPos;
in vec3 LightPos;   // extra in variable, since we need the light position in view space we calculate this in the vertex shader//灯源的位置

void main()
{
	//环境光照
	float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
	//漫反射光
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(LightPos - FragPos);

	float diff = max(dot(norm, lightDir), 0.0);//如果两个向量之间的角度大于90度，点乘的结果就会变成负数，这样会导致漫反射分量变为负数。
	vec3 diffuse = diff * lightColor;
	//镜面光照
	float specularStrength = 0.1;//镜面强度(Specular Intensity)
	vec3 viewDir = normalize(-FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);//reflect函数要求第一个向量是从光源指向片段位置的向量，所以取反
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);//32是高光的反光度(Shininess)，一个物体的反光度越高，反射光的能力越强，散射得越少，高光点就会越小。
	vec3 specular = specularStrength * spec * lightColor;


	vec3 result = (ambient + diffuse + specular) * objectColor;
	FragColor = vec4(result, 1.0);
}