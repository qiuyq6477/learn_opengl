#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec3 txtCoord;

void main()
{
//���½ǵ�λ��ֵΪ��������ɫֵ�ķ�Χ��0-1���������½�Ϊ��ɫ
    FragColor = vec4(txtCoord, 1.0f);
}