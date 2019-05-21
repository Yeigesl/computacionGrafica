#version 330 core
in vec4 ourColor;
out vec4 color;
//Variable global  el cual es el mismo y debe correposnder al deb
//main y debe ser positivo
uniform vec4 globalColor;
void main()
{
	color = globalColor * ourColor;
}