#version 430

#extension GL_ARB_geometry_shader4 : enable

layout(points) in;

layout(triangle_strip, max_vertices=16) out;

uniform mat4 View;

uniform mat4 Projection;

uniform vec3 CameraPos;

uniform float Radius;

in vec4 MyColor[1];

out vec4 fMyColor;

out vec2 Mapping;

const float PI = 3.1415926;

void main()

{
	mat4 mvp = Projection * View;

	vec3 pos = gl_PositionIn[0].xyz;


	vec3 toCamera = normalize(CameraPos - pos);

	vec3 up = vec3(0.0, Radius, 0.0);

	vec3 right = cross(toCamera, up);

    fMyColor = MyColor[0];


	//top right
	pos -= (right * 0.5);

    gl_Position = mvp * vec4(pos, 1.0);

    Mapping = vec2(1, 1);
    EmitVertex();



	// bottom right
	pos.y += Radius;

    gl_Position = mvp * vec4(pos, 1.0);

    Mapping = vec2(1, -1);
    EmitVertex();



	// top left
	pos.y -= Radius;
    pos += right;

    gl_Position = mvp * vec4(pos, 1.0);

    Mapping = vec2(-1, 1);
    EmitVertex();



	// bottom left
    pos.y += Radius;

    gl_Position = mvp * vec4(pos, 1.0);

    Mapping = vec2(-1, -1);
    EmitVertex();

    EndPrimitive();
}