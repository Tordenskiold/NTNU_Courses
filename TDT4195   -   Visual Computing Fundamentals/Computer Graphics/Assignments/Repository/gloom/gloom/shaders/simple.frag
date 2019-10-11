#version 430

in vec4 color;
in vec3 normal;

out vec4 colorOut;

void main()
{
	vec3 lightDirection = normalize(vec3(0.0, -0.5, -0.6));//normalize(vec3(0.8, -0.5, 0.6));
	float scalar = max(0, dot(normal, -lightDirection));
	colorOut = vec4(color.x * scalar, color.y * scalar, color.z * scalar, 1.0f);
}