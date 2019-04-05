#version 330 core

in vec3 our_color;
in vec3 our_normal;
out vec4 color;
in vec3 fragPos;

struct Positionlight{
    vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform Positionlight light;
//Especular
uniform vec3 viewPos;
void main(){
	vec3 ambient= light.ambient*our_color;
	vec3 normal =normalize(our_normal);
	vec3 lightDir=normalize(light.position-fragPos);
	float diff = max(dot(lightDir,normal),0.0);
    vec3 diffuse = diff*light.diffuse*our_color;
	// negativo se refleja desde arriba hacia la normal
	vec3 r= reflect(-lightDir,normal);
	//calucalo de la posicion del observador al fragemento 
	vec3 viewDir = normalize(viewPos-fragPos);
	float spec =pow(max(dot(r,viewDir),0.0),128.0); // 32 COMONENTE DE BRILLO mas grande la potencia más fina es la cirnfencia 
	vec3 specular =spec*light.specular*our_color;
	vec3 result=ambient+diffuse+specular;
	color = vec4(result, 1.0);

} 