#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <iostream>
#include "strukts.h"
#include "button.h"

using namespace std;
using namespace sf;

Color color_marking_grid;
Color color_line_intensity;
Color axes_color, axes_text_color;
Color spot_plate_info_ground_color, spot_plate_info_text_color;
Color cl_menu_change_button = Color::Green;
Color cl_menu_change_button_focus = Color::Magenta;
Color background_menu_change_charge = Color::Black;

float menu_change_charge_width = 200;

void change_color_style(int set, Color &background) {
	switch (set)
	{
	case 0://white mode
		background = Color::White;
		color_marking_grid = Color::Black;
		color_line_intensity = Color::Black;
		axes_color = Color::Black;
		axes_text_color = Color::Black;
		spot_plate_info_ground_color = { 213 , 244 , 11 };
		spot_plate_info_text_color = Color::Blue;
		break;
	case 1: // dark_mode
		background = Color::Black;
		color_marking_grid = Color::White;
		color_line_intensity = Color::White;
		axes_color = { 70,70,70 };
		axes_text_color = Color::White;
		spot_plate_info_ground_color = { 80,169,236 }; 
		spot_plate_info_text_color = Color::Green;
		break;
	case 2: // intensyv_mode
		background = Color::Black;
		color_marking_grid = Color::White;
		color_line_intensity = Color::White;
		axes_color = { 70,70,70 };
		axes_text_color = Color::White;
		spot_plate_info_ground_color = { 80,169,236 };
		spot_plate_info_text_color = Color::Green;
		zoom = 1;
		break;
	default:
		break;
	}
}

void draw_marking_grid(RenderWindow& window, View view, double meters_step_grid, int convert_P_to_M) {
	int width = view.getSize().x;
	int height = view.getSize().y;
	int left = view.getCenter().x - width / 2;
	int top = view.getCenter().y - height / 2;
	int down = top + height;
	int right = left + width;

	double pixels_step_grid = meters_step_grid * convert_P_to_M;
	VertexArray grid(Lines, (width + height) / pixels_step_grid * 2 + 100);

	point p_start = { left / convert_P_to_M, top / convert_P_to_M };
	p_start = get_grid_linked_position(p_start, meters_step_grid);
	int i = 0;
	for (int x = p_start.x * convert_P_to_M; x < right; x += pixels_step_grid) {
		grid[i] = Vector2f(x, top);
		grid[i].color = color_marking_grid;
		i++;
		grid[i] = Vector2f(x, down);
		grid[i].color = color_marking_grid;
		i++;
	}

	for (int y = p_start.y * convert_P_to_M; y < down; y += pixels_step_grid) {
		grid[i] = Vector2f(left, y);
		grid[i].color = color_marking_grid;
		i++;
		grid[i] = Vector2f(right, y);
		grid[i].color = color_marking_grid;
		i++;
	}

	window.draw(grid);
}

void draw_zarady(RenderWindow& window, vector <Zarad>& mass, int convert_P_to_M, double size_zar) {
	for (int i = 0; i < mass.size(); i++) {
		CircleShape zarad;
		Vector2f z_p;
		z_p = mass[i].coords;
		z_p.x *= convert_P_to_M;
		z_p.y *= -convert_P_to_M;
		z_p.x -= size_zar;
		z_p.y -= size_zar;
		zarad.setPosition(z_p);
		if (mass[i].zarad > 0)
			zarad.setFillColor(Color::Red);
		else
			zarad.setFillColor(Color::Blue);
		double rad = size_zar;
		zarad.setRadius(rad);
		window.draw(zarad);

		//RectangleShape rec(Vector2f(rad*2, rad*2));
		//rec.setPosition(z_p);
		//rec.setFillColor(Color::Green);
		//window.draw(rec);
	}
}

void debug_info(RenderWindow& window, Font& font, double time, FPS fps) {
	Text debug_info_out;
	debug_info_out.setFont(font);
	debug_info_out.setFillColor(spot_plate_info_text_color);

	string debug_stirng;

	debug_stirng = debug_stirng + "\n" + "FPS: " + to_string(fps.getFPS());
	debug_stirng = debug_stirng + "\n" + "Time: " + to_string(time) + L"seconds";
	debug_info_out.setString(debug_stirng);
	debug_info_out.setPosition(0, 0);


	View view_d = window.getDefaultView();
	view_d.setSize(window.getSize().x, window.getSize().y);
	view_d.setCenter(window.getSize().x / 2, window.getSize().y / 2);
	window.setView(view_d);

	window.draw(debug_info_out);
}

void draw_line(RenderWindow& window, vector <Vector2f> mass, double wigth, int convert_P_to_M, double zoom) {
	VertexArray x(TriangleStrip, mass.size() * 2);
	mass.push_back(mass[mass.size() - 1]);
	wigth /= zoom;
	for (int i = 0; i < mass.size(); i++) {
		mass[i].x *= convert_P_to_M;
		mass[i].y *= convert_P_to_M;
	}

	for (int i = 0; i < mass.size() - 1; i++) {

		double delta_x = mass[i].x - mass[i + 1].x;
		double delta_y = mass[i + 1].y - mass[i].y;
		double gip = sqrt(delta_x * delta_x + delta_y * delta_y);

		double x_strih = delta_y * wigth / gip; //cos(b)*wigth
		double y_strih = delta_x * wigth / gip; //sin(b)*wigth

		point p1 = { mass[i].x, mass[i].y };
		point p2 = { mass[i + 1].x, mass[i + 1].y };
		point f;
		point s;

		f.x = p1.x + x_strih;
		f.y = p1.y + y_strih;
		s.x = p1.x - x_strih;
		s.y = p1.y - y_strih;

		x[i * 2] = Vector2f(f.x, -f.y);
		x[i * 2 + 1] = Vector2f(s.x, -s.y);
		x[i * 2].color = color_line_intensity;
		x[i * 2 + 1].color = color_line_intensity;
	}

	window.draw(x);
}

void draw_coordinate_axes(RenderWindow& window, View view, Font font, double zoom, double step_grid_M,
	int convert_P_to_M) {
	//инициализация параметров линий координат
	int half_height_risk = 15 / zoom;
	int half_wight_risk = 5 / zoom;
	int half_wight_line = 3 / zoom;
	int offset_x_text = -20;
	int offset_y_text = 15;

	int width = view.getSize().x;
	int height = view.getSize().y;
	int left = view.getCenter().x - width / 2;
	int top = view.getCenter().y - height / 2;
	int down = top + height;
	int right = left + width;
	double step_grid_P = step_grid_M * convert_P_to_M;

	Text text;
	text.setFont(font);
	text.setFillColor(axes_text_color);
	text.setCharacterSize((int)25/zoom);

	//нахождение начальной точки оси Х
	point x_axis_start_pos;
	if (down < 0) {
		x_axis_start_pos = get_grid_linked_position(
			{ (double)left / convert_P_to_M, down / (double)convert_P_to_M }, step_grid_M);
		x_axis_start_pos.y -= 2 * step_grid_M;
	}
	else
		if (top > 0) {
			x_axis_start_pos = get_grid_linked_position(
				{ (double)left / convert_P_to_M, top / (double)convert_P_to_M }, step_grid_M);
			x_axis_start_pos.y += 2 * step_grid_M;
		}
		else
			x_axis_start_pos = get_grid_linked_position(
				{ (double)left / convert_P_to_M, 0 }, step_grid_M);
	//преобразование из точки привязано к сетке (в метрах) в пиксельные координаты
	// так как пиксельная ось Y инвертирована относительно декартовой(метрической) в коде выше сдвиг
	// сделан инвертировано т. к. пока я писал отрисовку осей я про это забыл
	// а функция которая по идее должна была всё исправить не заработала и мне вообще впадлу это переписывать
	// криво косо работает и бог с ним
	//с осью Y ситуация такая же
	Vector2f x_axis_risk =
		convert_point_to_coordsF(x_axis_start_pos, convert_P_to_M);
	
	//нахождение кол-ва вершин и инициализация массива вершин
	int lot_x_axis_nodes = (int)(width / step_grid_P) * 8 + 8;
	VertexArray X_axis(TriangleStrip, lot_x_axis_nodes);
	//необходимые переменные для отрисовки текста
	double x_pos = x_axis_start_pos.x;
	int counter_risk = -1;
	//это нужно для того что бы числа всегжа проходили через точку 0 по оси X
	if (abs(x_pos / step_grid_M / 2 - (int)(x_pos / step_grid_M / 2)) > 0.1) {
		counter_risk++;
		x_pos += step_grid_M;
	}
	//создание вершин оси Х
	for (int i = 0; i < lot_x_axis_nodes;) {
		Vector2f act_x_axis_risk_node;
		// верх линии входящей в риску
		act_x_axis_risk_node.x = x_axis_risk.x - half_wight_risk;
		act_x_axis_risk_node.y = x_axis_risk.y + half_wight_line;
		X_axis[i].color = axes_color;
		X_axis[i++].position = act_x_axis_risk_node;
		//низ линии входящей в риску
		act_x_axis_risk_node.y = x_axis_risk.y - half_wight_line;
		X_axis[i].color = axes_color;
		X_axis[i++].position = act_x_axis_risk_node;
		//верх начала риски
		act_x_axis_risk_node.y = x_axis_risk.y + half_height_risk;
		X_axis[i].color = axes_color;
		X_axis[i++].position = act_x_axis_risk_node;
		//низ начала риски
		act_x_axis_risk_node.y = x_axis_risk.y - half_height_risk;
		X_axis[i].color = axes_color;
		X_axis[i++].position = act_x_axis_risk_node;

		//верх конца риски
		act_x_axis_risk_node.x = x_axis_risk.x + half_wight_risk;
		act_x_axis_risk_node.y = x_axis_risk.y + half_height_risk;
		X_axis[i].color = axes_color;
		X_axis[i++].position = act_x_axis_risk_node;
		//низ конца риски
		act_x_axis_risk_node.y = x_axis_risk.y - half_height_risk;
		X_axis[i].color = axes_color;
		X_axis[i++].position = act_x_axis_risk_node;
		//верх линии выходящей из риски
		act_x_axis_risk_node.y = x_axis_risk.y + half_wight_line;
		X_axis[i].color = axes_color;
		X_axis[i++].position = act_x_axis_risk_node;
		//низ линии выходящей из риски
		act_x_axis_risk_node.y = x_axis_risk.y - half_wight_line;
		X_axis[i].color = axes_color;
		X_axis[i++].position = act_x_axis_risk_node;
		//всё для отрисовки текста и чередования его через 1 риску
		counter_risk++;
		if (counter_risk%2==0){
			text.setPosition(x_axis_risk.x + offset_x_text,
				x_axis_risk.y + offset_y_text);
			string pos_text = to_string(x_pos);
			pos_text.erase(4);
			text.setString(pos_text);
			window.draw(text);
			x_pos += 2 * step_grid_M;
		}
		
		x_axis_risk.x += step_grid_P;
	}
	window.draw(X_axis);
	//
	

	//нахождение начальной точки оси Y
	point y_axis_start_pos;
	if (left > 0) {
		y_axis_start_pos = get_grid_linked_position(
			{ (double)left / convert_P_to_M, top / (double)convert_P_to_M }, step_grid_M);
		y_axis_start_pos.x += 2 * step_grid_M;
	}
	else
		if (right < 0) {
			y_axis_start_pos = get_grid_linked_position(
				{ (double)right / convert_P_to_M, top / (double)convert_P_to_M }, step_grid_M);
			y_axis_start_pos.x -= 2 * step_grid_M;
		}
		else
			y_axis_start_pos = get_grid_linked_position(
				{ 0, (double)top / convert_P_to_M }, step_grid_M);
	Vector2f y_axis_risk =
		convert_point_to_coordsF(y_axis_start_pos, convert_P_to_M);
	//нахождение кол-ва вершин и инициализация массива вершин
	int lot_y_axis_nodes = (int)(height / step_grid_P) * 8 + 8;
	VertexArray Y_axis(TriangleStrip, lot_y_axis_nodes);
	//необходимые переменные для отрисовки текста
	double y_pos = y_axis_start_pos.y;
	int counter_risk_y = -1;
	//это нужно для того что бы числа всегжа проходили через точку 0 по оси Y
	if (abs(y_pos / step_grid_M / 2 - (int)(y_pos / step_grid_M / 2)) > 0.1) {
		counter_risk_y++;
		y_pos += step_grid_M;
	}
	//создание вершин оси Y
	for (int i = 0; i < lot_y_axis_nodes;) {
		Vector2f act_y_axis_risk_node;
		//право линии входящей в риску сверху
		act_y_axis_risk_node.y = y_axis_risk.y - half_wight_risk;
		act_y_axis_risk_node.x = y_axis_risk.x + half_wight_line;
		Y_axis[i].color = axes_color;
		Y_axis[i++].position = act_y_axis_risk_node;
		//лево линии входящей в риску сверху
		act_y_axis_risk_node.x = y_axis_risk.x - half_wight_line;
		Y_axis[i].color = axes_color;
		Y_axis[i++].position = act_y_axis_risk_node;
		//право верха риски
		act_y_axis_risk_node.x = y_axis_risk.x + half_height_risk;
		Y_axis[i].color = axes_color;
		Y_axis[i++].position = act_y_axis_risk_node;
		//лево верха риски
		act_y_axis_risk_node.x = y_axis_risk.x - half_height_risk;
		Y_axis[i].color = axes_color;
		Y_axis[i++].position = act_y_axis_risk_node;
		//право низа риски
		act_y_axis_risk_node.y = y_axis_risk.y + half_wight_risk;
		act_y_axis_risk_node.x = y_axis_risk.x + half_height_risk;
		Y_axis[i].color = axes_color;
		Y_axis[i++].position = act_y_axis_risk_node;
		//лево низа риски
		act_y_axis_risk_node.x = y_axis_risk.x - half_height_risk;
		Y_axis[i].color = axes_color;
		Y_axis[i++].position = act_y_axis_risk_node;
		//право линии выходящей из риски
		act_y_axis_risk_node.x = y_axis_risk.x + half_wight_line;
		Y_axis[i].color = axes_color;
		Y_axis[i++].position = act_y_axis_risk_node;
		//лево линии выходящей из риски
		act_y_axis_risk_node.x = y_axis_risk.x - half_wight_line;
		Y_axis[i].color = axes_color;
		Y_axis[i++].position = act_y_axis_risk_node;

		//всё для отрисовки текста и чередования его через 1 риску
		counter_risk_y++;
		if (counter_risk_y % 2 == 0) {
			text.setPosition(y_axis_risk.x - offset_x_text,
				y_axis_risk.y - offset_y_text);
			string pos_text = to_string(-y_pos);
			pos_text.erase(4);
			text.setString(pos_text);
			window.draw(text);
			y_pos += 2 * step_grid_M;
		}

		y_axis_risk.y += step_grid_P;
	}
	window.draw(Y_axis);
}

void draw_spot_information(RenderWindow& window, View view, 
	double zoom, Vector2f pixel_position,
	wstring information, Font font) {
	int offset = 10;
	//
	Text text;
	text.setFont(font);
	text.setFillColor(spot_plate_info_text_color);
	text.setCharacterSize((int)25);
	text.setString(information);
	
	//
	Vector2f size_tablet;
		size_tablet.y = text.getGlobalBounds().height*2;
		size_tablet.x = text.getGlobalBounds().width*2;
	VertexArray tablet(TrianglesStrip, 10);
	Vector2f tablet_apex;
	//левый фронт
	tablet_apex = pixel_position;
	tablet_apex.x -= size_tablet.x / 4 + offset;

	tablet_apex.y -= offset ;
	tablet[1].position = tablet_apex;
	tablet[1].color = spot_plate_info_ground_color;

	tablet_apex.y -= size_tablet.y;
	tablet[0].position = tablet_apex;
	tablet[0].color = spot_plate_info_ground_color;

	tablet_apex.x += offset;
	text.setPosition(tablet_apex);

	//левая часть середины (начало указателя позиции)
	tablet_apex = pixel_position;
	tablet_apex.x -= offset;

	tablet_apex.y -= offset;
	tablet[3].position = tablet_apex;
	tablet[3].color = spot_plate_info_ground_color;

	tablet_apex.y -= size_tablet.y;
	tablet[2].position = tablet_apex;
	tablet[2].color = spot_plate_info_ground_color;

	//середина
	tablet_apex = pixel_position;

	tablet[5].position = tablet_apex;
	tablet[5].color = spot_plate_info_ground_color;

	tablet_apex.y -= size_tablet.y + offset;
	tablet[4].position = tablet_apex;
	tablet[4].color = spot_plate_info_ground_color;

	//правая часть середины (начало указателя позиции)
	tablet_apex = pixel_position;
	tablet_apex.x += offset;

	tablet_apex.y -= offset;
	tablet[7].position = tablet_apex;
	tablet[7].color = spot_plate_info_ground_color;

	tablet_apex.y -= size_tablet.y;
	tablet[6].position = tablet_apex;
	tablet[6].color = spot_plate_info_ground_color;

	//правый фронт
	tablet_apex = pixel_position;
	tablet_apex.x += size_tablet.x / 4 + offset;

	tablet_apex.y -= offset;
	tablet[9].position = tablet_apex;
	tablet[9].color = spot_plate_info_ground_color;

	tablet_apex.y -= size_tablet.y;
	tablet[8].position = tablet_apex;
	tablet[8].color = spot_plate_info_ground_color;

	window.setView(view);
	window.draw(tablet);
	window.draw(text);
}


void draw_menu_change_charge(RenderWindow& window, vector <Zarad>& segment_vertex/*, vector <Button> btn,
	int charge_in_focus, int page_list_charge, vector <wstring> text*/){
	RectangleShape background({ menu_change_charge_width ,(float)(VideoMode::getDesktopMode().height / 1.6) });
	background.setFillColor(background_menu_change_charge);
	window.draw(background);
}