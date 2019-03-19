// FRAGMENT SHADER

#version 330

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

out vec4 v_position;
out vec2 v_texCoord;
out vec3 v_normal;

void main(void)
{
	vec4 resultColor = vec4(0.0, 0.0, 0.0, 0.0);
	vec4 eyePosition = vec4(0.0, 0.0, 0.0, 0.0);
	vec4 diffMatColor = texture2D(u_texture, v_texCoord);
	vec3 eyeVect = normalize(v_position.xyz - eyePosition.xyz);
	vec3 lightVect = normalize(reflect(lightVect, v_normal));
	vec3 reflectLight = normalize(reflect(lightVect, v_normal));
	float len = length(v_position.xyz - eyePosition.xyz);
	float specularFactor = 60.0; 
	float ambientFactor = 0.1;
	
	// finish the line
	vec4 diffColor = diffMatColor * u_lightPower * max(0.0, dot(v_normal, -lightVect)) / (1.0 + 0.25 *
	resultColor += diffColor;
	vec4 ambientColor = ambientFactor * diffMatColor;
	resultColor += ambientColor;
	// finish the line
	vec4 specularColor = vec4(1.0, 1.0, 1.0, 1.0) * u_lightPower * pow(max(0.0, dot(reflectLight, -eye
	resultColor += specularColor;
	
	gl_FragColor = resultColor;
	
}