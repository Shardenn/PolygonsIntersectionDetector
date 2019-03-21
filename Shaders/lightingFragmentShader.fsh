#version 130

// uniform vec4 ambientColor;
// uniform vec4 diffuseColor;
// uniform vec4 specularColor;
// uniform float ambientReflection;
// uniform float diffuseReflection;
// uniform float specularReflection;
// uniform float shininess;
uniform sampler2D u_texture;
uniform vec4 u_lightPosition;
uniform float u_lightPower;

// in vec3 varyingLightDirection;
// in vec3 varyingViewerDirection;

in vec4 v_position;
in vec2 v_textureCoord;
in vec3 v_normal;

void main(void)
{
	vec4 resultColor = vec4(0.0, 0.0, 0.0, 0.0);
	vec4 eyePosition = vec4(0.0, 0.0, 0.0, 1.0);
	vec4 diffMatColor = texture2D(u_texture, v_textureCoord);
	vec3 eyeVect = normalize(v_position.xyz - eyePosition.xyz);
	vec3 lightVect = normalize(v_position.xyz - u_lightPosition.xyz);
	vec3 reflectLight = normalize(reflect(lightVect, v_normal));
	float len = length(v_position.xyz - eyePosition.xyz);
	float specularFactor = 60.0; 
	float ambientFactor = 0.1;
	
	vec4 diffColor = diffMatColor * u_lightPower * max(0.0, dot(v_normal, -lightVect)) / (1.0 + 0.25 * pow(len, 2));
	resultColor += diffColor;
	vec4 ambientColor = ambientFactor * diffMatColor;
	resultColor += ambientColor;
	vec4 specularColor = vec4(1.0, 1.0, 1.0, 1.0) * u_lightPower * pow(max(0.0, dot(reflectLight, -eyeVect)), specularFactor) / (1.0 + 0.25 * pow(len, 2));
	resultColor += specularColor;
	
	gl_FragColor = resultColor;
	
}
