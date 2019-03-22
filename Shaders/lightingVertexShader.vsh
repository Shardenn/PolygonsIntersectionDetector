#version 130

uniform mat4 u_projectionMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_modelMatrix;

// uniform mat3 normalMatrix;
// uniform vec3 lightPosition;

in vec4 vertexPosition;
in vec3 normal;
in vec2 textureCoordinate;

out vec4 v_position;
out vec3 v_normal;
out vec2 v_textureCoord;

// out vec3 varyingLightDirection;
// out vec3 varyingViewerDirection;

void main(void)
{
    mat4 mvMatrix = u_viewMatrix * u_modelMatrix;

    v_textureCoord = textureCoordinate;
    v_normal = normalize(vec3(mvMatrix * vertexPosition));
    v_position = mvMatrix * vertexPosition;

    gl_Position = vec4(vertexPosition);
    //gl_Position = u_projectionMatrix * mvMatrix * vertexPosition;
}
