#version 330
const float ambient = .5f;
uniform float lightIntensity;

uniform bool enableDiffuse;
uniform bool enableSpecular;

uniform vec3 lightPosition;
uniform vec3 eyePosition;
uniform int shininess;
uniform sampler2D tex;

in vec3 fnormal;
in vec3 fposition;
in vec4 fcolor;
in vec2 fuv;
out vec4 color_out;

void main() {
  vec3 normal = normalize(fnormal);
  vec3 dirToLight = normalize(lightPosition - vec3(fposition));
  vec3 vertexToEye = normalize(eyePosition - vec3(fposition));

  vec4 TextureColor = texture(tex, fuv) * ambient;
  vec4 AmbientColor = TextureColor;
  vec4 DiffuseColor = vec4(0);
  vec4 SpecularColor = vec4(0);

  if(enableDiffuse) {
      float diffuseFactor = clamp( dot(normal, dirToLight), 0.0, 1.0 );
      DiffuseColor = (AmbientColor * (diffuseFactor));
  }

  if(enableSpecular) {
      vec3 LightReflect = normalize(reflect(-dirToLight, normal));
      float specularFactor = pow(clamp(dot(vertexToEye, LightReflect), 0.0, 1.0), shininess);
      SpecularColor = (AmbientColor * (specularFactor));
  }

  color_out =  (DiffuseColor + SpecularColor + AmbientColor) * lightIntensity ;
}
