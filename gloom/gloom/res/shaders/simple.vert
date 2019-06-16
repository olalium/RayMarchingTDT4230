#version 450 core

in layout(location = 0) vec3 position;
in layout(location = 1) vec3 normal_in;
in layout(location = 2) vec2 texCoords;
in layout(location = 3) vec3 texCoords3D;

uniform layout(location = 3) mat4 MVP;
uniform layout(location = 4) mat4 MV;
uniform layout(location = 7) int is3D;
uniform layout(location = 9) vec3 positionSun;
uniform layout(location = 10) vec3 positionObs;

out layout(location = 0) vec3 normal_out;
out layout(location = 1) vec3 v;
out layout(location = 2) vec2 texCoords_out;
out layout(location = 3) vec3 texCoords3D_out;
out layout(location = 12) mat2x3 lightPositions;
out layout(location = 4) vec3 vp;

void main()
{
	texCoords_out = texCoords;
	texCoords3D_out = texCoords3D;

	v = vec3(MV * vec4(position, 1.0f));
    normal_out = normalize(vec3(MV * vec4(normal_in,0.0)));
    lightPositions = mat2x3(positionSun, positionObs);
    gl_Position = MVP * vec4(position, 1.0f);
    if (is3D == 1) {
    	vp = vec3(MVP * vec4(position, 1.0f));
	}
}
