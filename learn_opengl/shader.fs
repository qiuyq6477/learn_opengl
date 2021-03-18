#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;    
    float shininess;
}; 

struct Light {
    vec3  position;
    vec3  direction;
    float cutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;
  
uniform Material material;
uniform Light light;
uniform vec3 viewPos;

void main()
{
	vec3 lightDir = normalize(light.position - FragPos);
	float theta = dot(lightDir, normalize(-light.direction));
	if(theta > light.cutOff) 
	{       
	  // ִ�й��ռ���
	  // ambient
		vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
  	
		// diffuse 
		vec3 norm = normalize(Normal);
		vec3 lightDir = normalize(light.position - FragPos);
		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));  
    
		// specular
		vec3 viewDir = normalize(viewPos - FragPos);
		vec3 reflectDir = reflect(-lightDir, norm);  
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
		vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));  
        
		vec3 result = ambient + diffuse + specular;
		FragColor = vec4(result, 1.0);
	}
	else  // ����ʹ�û����⣬�ó����ھ۹�֮��ʱ��������ȫ�ڰ�
		FragColor= vec4(light.ambient * vec3(texture(material.diffuse, TexCoords)), 1.0);
   
} 