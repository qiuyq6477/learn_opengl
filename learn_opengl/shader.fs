#version 330 core
out vec4 FragColor;

struct Material { 
    vec3 ambient; //ambient���������������ڻ���������������巴�����ʲô��ɫ��ͨ�����Ǻ�������ɫ��ͬ����ɫ��
    vec3 diffuse; //diffuse����������������������������������ɫ�����ͻ�������һ������������ɫҲҪ����Ϊ������Ҫ��������ɫ��
    vec3 specular; //specular�����������õ��Ǿ�����ն��������ɫӰ�죨�����������ܷ���һ�������ض��ľ���߹���ɫ��
    float shininess; //�����(Shininess)��shininessӰ�쾵��߹��ɢ��/�뾶��
}; 
uniform Material material;

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;

in vec3 Normal;
in vec3 FragPos;
in vec3 LightPos;   // extra in variable, since we need the light position in view space we calculate this in the vertex shader//��Դ��λ��

uniform vec3 objectColor;

void main()
{
	 // ������
    vec3 ambient = light.ambient * material.ambient;

    // ������ 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(LightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);

    // �����
    vec3 viewDir = normalize(-FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);  

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}