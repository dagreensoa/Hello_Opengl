#version 330 core
layout (location = 0) out vec4 out_color;

in vec3 v_color;
in vec3 v_position;

uniform float time; //從CPU傳送到著色器的變數

void main()
{
	out_color = vec4( (sin(time + v_position) + 1)/2,1);


}