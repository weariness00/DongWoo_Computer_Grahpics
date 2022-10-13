#version 330 core

uniform vec4 vColor;
in vec4 prismColor;

out vec4 FragColor; //--- 프레임 버퍼로 출력
void main()
{
	if(prismColor.w > 0)
		FragColor = prismColor;
	else
		FragColor = vColor;
}