#version 430

// Буфер для хранения минимального и максимального значения
struct Charge{
    vec2 position;
    float charge;
};

uniform Charge charge[100];
uniform int charge_size;
uniform float convert_P_to_M;
uniform float x_offset;
uniform float y_offset;
uniform float max_intensyty;
uniform float min_intensyty;

void main()
{
    float intensity_field = 0;
    for (int i = 0; i < charge_size; i++) {
		float delta_x = charge[i].position.x - (gl_FragCoord.xy.x + x_offset)/convert_P_to_M;
		float delta_y = charge[i].position.y - (gl_FragCoord.xy.y + y_offset)/convert_P_to_M;
		float r2 = delta_y * delta_y + delta_x * delta_x;
		intensity_field += charge[i].charge / r2;
	}
	if (intensity_field > 0)
		gl_FragColor = vec4(intensity_field / max_intensyty, 0, 0, 1);
	else
		gl_FragColor = vec4(0, 0, -intensity_field / max_intensyty, 1);
}
