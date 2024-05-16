#version 430

uniform sampler2D texture_inter;
uniform float max_intensyty;
uniform float min_intensyty;

void main(){
	vec4 color = texture(texture_inter, gl_FragCoord.xy);
	float intensyty = color.r;
	if (intensyty > 0)
		gl_FragColor = vec4(0, 0, intensyty / max_intensyty, 1);
	else
		gl_FragColor = vec4(intensyty / min_intensyty, 0, 0, 1);
}
