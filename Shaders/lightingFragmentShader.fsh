#version 130

// uniform vec4 ambientColor;
// uniform vec4 diffuseColor;
// uniform vec4 specularColor;
// uniform float ambientReflection;
// uniform float diffuseReflection;
// uniform float specularReflection;
// uniform float shininess;
//uniform sampler2D u_texture;
uniform vec3 u_lightPosition;
uniform float u_lightPower;
uniform vec3 u_lightColor;
uniform vec3 u_objectColor;

// in vec3 varyingLightDirection;
// in vec3 varyingViewerDirection;

in vec3 v_position;
in vec3 v_normal;
in vec2 v_textureCoord;

void main(void)
{
    float ambientStrength = 0.5f;
    vec3 ambient = ambientStrength * u_lightColor;

    vec3 norm = normalize(v_normal);
    vec3 lightDirection = normalize(u_lightPosition - v_position);

    float diff = max(0.0, dot(norm, lightDirection));
    vec3 diffuse = diff * u_lightColor;

    vec3 result = (ambient + diffuse) * u_objectColor;
    gl_FragColor = u_lightPower * vec4(result, 1.0);
    //gl_FragColor = vec4(norm, 1.0);
}
