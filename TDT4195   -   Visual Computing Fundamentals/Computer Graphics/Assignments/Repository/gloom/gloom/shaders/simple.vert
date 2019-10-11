#version 430

layout(location=0) in vec3 position;
layout(location=1) in vec4 colorIn;
layout(location=2) in vec3 normalIn;
layout(location=5) uniform mat4 transMatrix;
layout(location=6) uniform mat4 modelMatrix;

out vec4 color;
out vec3 normal;

void main()
{
    gl_Position = transMatrix * vec4(position, 1.0f);
	color = colorIn;
	normal = normalize(mat3(modelMatrix) * normalIn);
}