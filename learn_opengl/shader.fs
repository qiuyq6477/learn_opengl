#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

// texture samplers
uniform sampler2D texture1;
uniform sampler2D texture2;//供纹理对象使用的内建数据类型，采样器(Sampler)

void main()
{
	// linearly interpolate between both textures (80% container, 20% awesomeface)
	//进行纹理采样，第一个参数是纹理采样器，第二个参数是对应的纹理坐标。
    //texture函数会使用之前设置的纹理参数对相应的颜色值进行采样。
	FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
}