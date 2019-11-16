#version 330

in vec2 _uv;
in float af;
in float df;
in float sf;

uniform vec3 sun_color;
uniform sampler2D diffuse_texture;

float SUN_POWER = 0.8; // TODO: MAKE THIS A CONFIGURABLE UNIFORM IN SHADER LATER ON
// WE CAN MAKE THIS SHADER LEVEL OR GLOBAL LEVEL IN SUN NAMESPACE

out vec4 fragment_color;

void main()
{
	vec3 color = texture(diffuse_texture, _uv).rgb;

	vec3 ambient = color * af;
	vec3 diffuse = color * df;
	vec3 specular = sun_color * sf;
	vec3 pixel = ambient + diffuse + specular;

	fragment_color = vec4(pixel * SUN_POWER, 1.0);
	//fragment_color = vec4(1,0,0,1);
}
