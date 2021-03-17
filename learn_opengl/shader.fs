#version 330 core
out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 viewPos;

in vec3 Normal;
in vec3 FragPos;
in vec3 LightPos;   // extra in variable, since we need the light position in view space we calculate this in the vertex shader//��Դ��λ��

void main()
{
	//��������
	float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
	//�������
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(LightPos - FragPos);

	float diff = max(dot(norm, lightDir), 0.0);//�����������֮��ĽǶȴ���90�ȣ���˵Ľ���ͻ��ɸ����������ᵼ�������������Ϊ������
	vec3 diffuse = diff * lightColor;
	//�������
	float specularStrength = 0.1;//����ǿ��(Specular Intensity)
	vec3 viewDir = normalize(-FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);//reflect����Ҫ���һ�������Ǵӹ�Դָ��Ƭ��λ�õ�����������ȡ��
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);//32�Ǹ߹�ķ����(Shininess)��һ������ķ����Խ�ߣ�����������Խǿ��ɢ���Խ�٣��߹��ͻ�ԽС��
	vec3 specular = specularStrength * spec * lightColor;


	vec3 result = (ambient + diffuse + specular) * objectColor;
	FragColor = vec4(result, 1.0);
}