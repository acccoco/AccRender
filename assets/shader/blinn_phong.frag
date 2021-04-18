#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;


struct PointLight {
    vec3 pos;
    vec3 dir;
    vec3 color;
};

uniform sampler2D texture_diffuse_0;
uniform PointLight plight0;
uniform vec3 eye_pos;


void main()
{
    float constant = 1.0f;
    float linear = 0.32f;
    float quadratic = 0.08f;

    vec3 light_dir = normalize(plight0.dir);
    vec3 view_dir = normalize(FragPos - eye_pos);
    vec3 normal = normalize(Normal);
    vec3 reflect_dir = normalize(reflect(light_dir, normal));
    float distance = length(plight0.pos - FragPos);
    vec3 halfway = -normalize(light_dir + view_dir);

    float diffuse_coef = max(0.0, dot(-light_dir, normal));

    // blinn-phong 和 phong 两种方式计算高光
    bool blinn = true;
    float specular_coef = 0.f;
    if (blinn)
        specular_coef = pow(max(0.0, dot(normal, halfway)), 128.0);
    else
        specular_coef = pow(max(0.0, dot(-view_dir, reflect_dir)), 128.0);
    
    float light_attenuation = 1.0 / (constant + linear * distance + quadratic * distance * distance);

    vec3 diffuse_color = texture(texture_diffuse_0, TexCoord).rbg * diffuse_coef * plight0.color;
    vec3 specular_color = vec3(1.f, 1.f, 1.f) * specular_coef;

    gl_FragColor = vec4((diffuse_color + specular_color) * light_attenuation, 1.0f);
}
