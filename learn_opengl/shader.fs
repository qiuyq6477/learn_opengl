#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec3 txtCoord;

void main()
{
//左下角的位置值为负数，颜色值的范围在0-1，所以左下角为黑色
    FragColor = vec4(txtCoord, 1.0f);
}