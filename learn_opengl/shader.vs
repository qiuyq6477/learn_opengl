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
   FragPos = vec3(view * model * vec4(aPos, 1.0));//����������ռ��е�λ��
   Normal = mat3(transpose(inverse(view * model))) * aNormal;//����ķ�����
   LightPos = vec3(view * vec4(lightPos, 1.0));// Transform world-space light position to view-space light position

   //��������
	float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
	//�������
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(LightPos - FragPos);

	float diff = max(dot(norm, lightDir), 0.0);//�����������֮��ĽǶȴ���90�ȣ���˵Ľ���ͻ��ɸ����������ᵼ�������������Ϊ������
	vec3 diffuse = diff * lightColor;
	//�������
	float specularStrength = 2;//����ǿ��(Specular Intensity)
	vec3 viewDir = normalize(-FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);//reflect����Ҫ���һ�������Ǵӹ�Դָ��Ƭ��λ�õ�����������ȡ��
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);//32�Ǹ߹�ķ����(Shininess)��һ������ķ����Խ�ߣ�����������Խǿ��ɢ���Խ�٣��߹��ͻ�ԽС��
	vec3 specular = specularStrength * spec * lightColor;


	color = (ambient + diffuse + specular) * objectColor;

};