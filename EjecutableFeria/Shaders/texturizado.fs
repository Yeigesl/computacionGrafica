#version 330 core

in vec2 our_text;
out vec4 color;
uniform sampler2D ourTexture;

void main(){
	
	color = texture(ourTexture, our_text);
	// a corresponde a la componenee alfa se verifica para descartar 
	if(color.a<=0.1)
		discard;
	//color = vec4(0.3, 0.5, 0.4, 1.0);
}