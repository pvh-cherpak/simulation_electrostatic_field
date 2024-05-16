#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <iostream>
#include "strukts.h"

using namespace std;
using namespace sf;

Vector2i convert_F_to_I(Vector2f p) {
	int x = p.x;
	int y = p.y;
	return { x ,y };
}

Vector2f convert_point_to_coordsF(point p, int convert_P_to_M) {
	Vector2f ans;
	//функция работает не правильно она не учитывает инвертацию оси Y
	// при пернводи из декартовых(метровые) в оконные координаты(пиксельные)
	ans.x = p.x * convert_P_to_M;
	ans.y = p.y * convert_P_to_M;
	return ans;
}

Vector2f convert_point_to_coordsF_correct(point p, int convert_P_to_M) {
	Vector2f ans;
	//функция работает не правильно она не учитывает инвертацию оси Y
	// при пернводи из декартовых(метровые) в оконные координаты(пиксельные)
	ans.x = p.x * convert_P_to_M;
	ans.y = p.y * -convert_P_to_M;
	return ans;
}


Vector2f convert_coords_to_pixelsF(Vector2f p, int convert_P_to_M) {
	Vector2f ans;
	ans.x = p.x * convert_P_to_M;
	ans.y = -p.y * convert_P_to_M;
	return ans;
}

Vector2i convert_pixel_to_coordsI(Vector2f p, int convert_P_to_M) {
	int x = p.x / convert_P_to_M;
	int y = -p.y / convert_P_to_M;
	return { x , y };
}

Vector2f convers_pixel_to_coordsF(Vector2f p, int convert_P_to_M) {
	p.x = p.x / convert_P_to_M;
	p.y = -p.y / convert_P_to_M;
	return p;
}

point get_grid_linked_position(point p, double meters_step_grid) {
	p.x = round(p.x / meters_step_grid) * meters_step_grid;
	p.y = round(p.y / meters_step_grid) * meters_step_grid;
	return p;
}

Vector2f get_grid_linked_positionF(Vector2f p, double meters_step_grid) {
	p.x = round(p.x / meters_step_grid) * meters_step_grid;
	p.y = round(p.y / meters_step_grid) * meters_step_grid;
	return p;
}

Zarad add_zarad(Vector2f coords, int convert_P_to_M, 
	double step_grid_M, bool show_marking_grid) {
	Vector2f p;
	p.x = coords.x / convert_P_to_M;
	p.y = -coords.y / convert_P_to_M;
	Zarad z;
	if (show_marking_grid)
		z.coords = get_grid_linked_positionF(p, step_grid_M);
	else
		z.coords = p;
	return z;
}

Vector2f vector_projection(double module, Vector2f vector_start_p, Vector2f zero_point) {
	double delta_r_vect_x = vector_start_p.x - zero_point.x;
	double delta_r_vect_y = vector_start_p.y - zero_point.y;
	double gip = sqrt(delta_r_vect_x * delta_r_vect_x + delta_r_vect_y * delta_r_vect_y);

	Vector2f projection;
	projection.x = module * delta_r_vect_x / gip;
	projection.y = module * delta_r_vect_y / gip;
	return projection;
}

vector <Vector2f> calculation_tension_line(Vector2f act_point, bool zar_is_negative,
	vector<Zarad> mass, double step, int lot_step) {
	vector <Vector2f> ans;
	ans.push_back(act_point);
	ans.reserve(lot_step);
	for (double i = step; i < step * lot_step; i += step) {
		Vector2f proje;
		for (int j = 0; j < mass.size(); j++) {
			double delta_x = mass[j].coords.x - act_point.x;
			double delta_y = mass[j].coords.y - act_point.y;
			double r = delta_y * delta_y + delta_x * delta_x;
			double modul = mass[j].zarad / r ;

			Vector2f P_p = vector_projection(modul, act_point, mass[j].coords);
			proje.x += P_p.x;
			proje.y += P_p.y;
		}
		if (zar_is_negative) {
			proje.x = -proje.x;
			proje.y = -proje.y;
		}
		double g = sqrt(proje.x * proje.x + proje.y * proje.y);
		ans.push_back(act_point);
		act_point.x += 0.1 * proje.x / g;
		act_point.y += (0.1 * proje.y) / g;
	}
	return ans;
}

vector <vector <Vector2f>> get_new_tensor_line_cadr(vector <Zarad> &zarady) {
	vector <vector <Vector2f>> line_cadr;
	line_cadr.reserve(zarady.size() * 10);
	for (int i = 0; i < zarady.size(); i++)
		for (double angle = 0; angle < 2 * acos(-1); angle += acos(-1) / 5) {
			Vector2f v = zarady[i].coords;
			v.x += cos(angle) * 0.01;
			v.y += sin(angle) * 0.01;
			line_cadr.push_back(calculation_tension_line(v, zarady[i].zarad < 0, 
				zarady, 1, 1000));
		}
	return line_cadr;
}

double get_pow_number(double number) {
	double power = 1;
	if (abs(number) < 1) {
			while (abs(number) * 10 < 10) {
				number *= 10;
				power*=0.1;
			}
	}
	else {
			while (abs(number) / 10 > 1) {
				number /= 10;
				power *=10;
			}
	}
	return power;
}

wstring get_readable_numbers(double number, bool standard_view, int precis,vector <wstring> &prefix) {
	int count_digit = 0;
	if (abs(number) < 1) {
		if (!standard_view)
			while (abs(number) * 1000 < 1000 && count_digit > -12) {
				number *= 1000;
				count_digit -= 3;
			}
		else
			while (abs(number) * 10 < 10) {
				number *= 10;
				count_digit--;
			}
	}
	else {
		if (!standard_view)
			while (abs(number) / 1000 > 1 && count_digit < 12) {
				number /= 1000;
				count_digit += 3;
			}
		else
			while (abs(number) / 10 > 1) {
				number /= 10;
				count_digit++;
			}
	}

	for (int i = 0; i < precis; i++)
		number *= 10;
	number = round(number);
	for (int i = 0; i < precis; i++)
		number /= 10;

	wstring ans = to_wstring(number);

	if (standard_view) {
		if (number < 0)
			ans.erase(3 + precis);
		else
			ans.erase(2 + precis);
		ans = ans + L"*10^(" + to_string(count_digit) + ") ";
	}
	else {
		if (number < 0)
			ans.erase(5 + precis);
		else
			ans.erase(4 + precis);
		ans = ans + L" " + prefix[4 + count_digit / 3];
	}
	return ans;
}

wstring get_spot_information(Vector2f coords, vector <Zarad>& zarady, bool standard_view, vector <wstring> &strings, vector <wstring>& prefix) {
	wstring information;

	//вычесления напряжённости и потенциала поля в этой точке
	double intensity_field = 0;
	double potential_field = 0;
	for (int j = 0; j < zarady.size(); j++) {
		double delta_x = zarady[j].coords.x - coords.x;
		double delta_y = zarady[j].coords.y - coords.y;
		double r2 = delta_y * delta_y + delta_x * delta_x;
		intensity_field += zarady[j].zarad / r2;
		potential_field += zarady[j].zarad / sqrt(r2);
	}
	intensity_field *= 9e9;
	potential_field *= 9e9;

	information += strings[0];
	information += get_readable_numbers(intensity_field, standard_view, 3, prefix) + strings[1] + L'\n';
	information += strings[2];
	information += get_readable_numbers(potential_field, standard_view, 3, prefix) + strings[3];

	return information;
}

vector <wstring> spot_info_text;
vector <wstring> prefix;

void calculate_new_charges_position(vector <Zarad>& zarady, double time_charges_anim) {
	for (int i = 0; i < zarady.size(); i ++) {
		if (zarady[i].is_locked)
			continue;
		Vector2f proje;
		for (int j = 0; j < zarady.size(); j++) {
			if (i == j)
				continue;
			double delta_x = zarady[j].coords.x - zarady[i].coords.x;
			double delta_y = zarady[j].coords.y - zarady[i].coords.y;
			double r = delta_y * delta_y + delta_x * delta_x;
			double modul;
			modul = zarady[j].zarad * zarady[i].zarad * 9e9 / r;

			Vector2f P_p = vector_projection(modul, zarady[i].coords, zarady[j].coords);
			proje.x += P_p.x;
			proje.y += P_p.y;
		}
		/*if (zarady[i]*zarady[j]) {
			proje.x = -proje.x;
			proje.y = -proje.y;
		}*/
		zarady[i].acceleration.x = proje.x / zarady[i].mass;
		zarady[i].acceleration.y = proje.y / zarady[i].mass;

		zarady[i].coords.x += zarady[i].speed.x * time_charges_anim + zarady[i].acceleration.x * time_charges_anim
			* time_charges_anim / 2;
		zarady[i].coords.y += zarady[i].speed.y * time_charges_anim + zarady[i].acceleration.y * time_charges_anim
			* time_charges_anim / 2;

		zarady[i].speed.x += zarady[i].acceleration.x * time_charges_anim;
		zarady[i].speed.y += zarady[i].acceleration.y * time_charges_anim;
	}

	
}

void collision_of_charges(vector <Zarad>& zarady) {
	for (int i = 0; i < zarady.size(); i++)
		for (int j = i + 1; j < zarady.size(); j++) {
			double delta_x = zarady[j].coords.x - zarady[i].coords.x;
			double delta_y = zarady[j].coords.y - zarady[i].coords.y;
			double r = delta_y * delta_y + delta_x * delta_x;
			if (r < 0.2) {
				zarady[j].speed.x = -zarady[j].speed.x;
				zarady[i].speed.x = -zarady[i].speed.x;
				zarady[j].speed.y = -zarady[j].speed.y;
				zarady[i].speed.y = -zarady[i].speed.y;
			}
		}	
}

wstring get_charge_information(Zarad& charge, vector <wstring>& charge_text, bool standart_vies) {
	wstring str = charge_text[0] + get_readable_numbers(charge.zarad, standart_vies, 3, prefix) + charge_text[1] + "\n";
	str = str + charge_text[2] + get_readable_numbers(charge.mass, standart_vies, 3, prefix) + charge_text[3] + "\n";
	str = str + charge_text[4];
	if (charge.is_locked)
		str = str + charge_text [5] + "\n";
	else
		str = str + charge_text[6] + "\n";

	double speed = sqrt(charge.speed.x * charge.speed.x + charge.speed.y * charge.speed.y);
	str = str + charge_text[7] + get_readable_numbers(speed, standart_vies, 3, prefix) + charge_text[8];

	return str;
}