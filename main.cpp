#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <iostream>
#include "functions.h"
#include "visual_function.h"
#include "strukts.h"
#include "button.h"
#include "textbox.h"
#include <io.h>
#include <fcntl.h>
#include <cstdlib>
#include <fstream>

using namespace sf;
using namespace std;

void GUI_menu(RenderWindow& window, Font& font, Color& background, vector <int>& changed_falgs);


void FAQ_list(RenderWindow& window, Texture& texture_exit_selected, Texture& act_texture_exit , Color background,
	 Color text_color, Font& font);

int main()
{
	_setmode(_fileno(stdout), _O_U16TEXT);
	_setmode(_fileno(stdin), _O_U16TEXT);
	_setmode(_fileno(stderr), _O_U16TEXT);

	ContextSettings settings;
	settings.antialiasingLevel = 8;
	RenderWindow window(VideoMode(VideoMode::getDesktopMode().width / 1.6,
		VideoMode::getDesktopMode().height / 1.6), "SFML Works!", Style::Default, settings);
	View pole_view = window.getDefaultView();
	pole_view.setCenter(0, 0);
	View interfase_view = window.getDefaultView();

	Font TimesNewRoman;
	if (!TimesNewRoman.loadFromFile("TimesNewROman.ttf"))
		cout << "error in load font TimesNewRoman" << endl;

	Font Arial;
	if (!Arial.loadFromFile("arial.ttf"))
		cout << "error in load font Arial" << endl;

	Font Sans;
	if (!Sans.loadFromFile("Comic Sans MS.ttf"))
		cout << "error in load font Comic Sans MS" << endl;

	Texture settings_1;
	if (!settings_1.loadFromFile("settings_1.png"))
	{
		cout << "петрович мы обосрались текстура не грузиться" << endl;
	}
	Button settings_button(settings_1, { 150, 150 });
	settings_button.setPosition({ interfase_view.getSize().x - 75,75});
	settings_button.set_center_in_center();

	
	//массив зарядов
	vector <Zarad> zarady;
	zarady.push_back({ Vector2f(0,0),1e-8 });

	//массив точек линий напряжённости
	vector <vector <Vector2f>> tensor_line_cadr_points;
	tensor_line_cadr_points = get_new_tensor_line_cadr(zarady);

	//массив точек концов отрезков
	vector <Zarad> segment_vertex;

	Color background;
	int number_color_style = 0; //1-black, 0-white, 3-not work yet

	//объявление всяких флагов меню
	bool show_marking_grid = true;
	bool show_spot_inform = false;
	bool standart_vies = false;
	bool settings_in_focus = false;
 
	//объявление всяких конвертирующих коэффициентов
	double step_grid_M = 0.5;//шаг сетки в метрах
	double zoom = 1;
	int convert_P_to_M = 100;
	double size_zar = 10;
	int number_change_zarad = 0;
	Vector2f position_spot_info = { 0,0 };


	FPS fps;
	change_color_style(number_color_style, background);

	Clock clock;

	// Главный цикл приложения. Выполняется, пока открыто окно
	while (window.isOpen())
	{
		window.clear(background);
		window.setView(pole_view);

		// Обрабатываем очередь событий в цикле
		Event event;
		while (window.pollEvent(event))
		{
			// Пользователь нажал на «крестик» и хочет закрыть окно?
			if (event.type == Event::Closed)
				window.close();
			if (event.type == Event::Resized)
			{
				// update the pole_view to the new size of the window
				FloatRect visibleArea(0., 0., event.size.width, event.size.height);
				pole_view.reset(visibleArea);
				pole_view.setCenter(0, 0);
				zoom = window.getSize().x / pole_view.getSize().x;
				//window.set
			}

			if (event.type == Event::KeyPressed) {
				if (event.key.code == Keyboard::Left)
					pole_view.move(-10.f, 0.f);
				if (event.key.code == Keyboard::Right)
					pole_view.move(10.f, 0.f);
				if (event.key.code == Keyboard::Up)
					pole_view.move(0.f, -10.f);
				if (event.key.code == Keyboard::Down)
					pole_view.move(0.f, 10.f);
				if (event.key.code == Keyboard::PageUp) {
					pole_view.zoom(0.9f);
					zoom = window.getSize().x / pole_view.getSize().x;
					size_zar *= 0.9;
				}
				if (event.key.code == Keyboard::PageDown) {
					pole_view.zoom(1.1f);
					zoom = window.getSize().x / pole_view.getSize().x;
					size_zar *= 1.1;
				}
			}

			if (event.type == Event::KeyReleased) {
				if (event.key.code == Keyboard::Q) {
					number_change_zarad = zarady.size();
					zarady.push_back(add_zarad(window.mapPixelToCoords(Mouse::getPosition(window)),
						convert_P_to_M, step_grid_M, show_marking_grid));
					tensor_line_cadr_points = get_new_tensor_line_cadr(zarady);
				}
				//if (event.key.code == Keyboard::D) {
				//	cout << "left: " << pole_view.getViewport().left<<endl;
				//	cout << "top: " << pole_view.getViewport().top << endl;
				//	cout << "height: " << pole_view.getSize().x << endl;
				//	cout << endl;
				//}
				if (event.key.code == Keyboard::Delete)
					if (number_change_zarad < zarady.size()) {
						zarady.erase(zarady.begin() + number_change_zarad);
						number_change_zarad = zarady.size();
						tensor_line_cadr_points = get_new_tensor_line_cadr(zarady);
					}
				if (event.key.code == Keyboard::G) {
					show_marking_grid = !show_marking_grid;
				}
				if (event.key.code == Keyboard::Equal) {
					step_grid_M += 0.1;
				}
				if (event.key.code == Keyboard::Hyphen) {
					step_grid_M -= 0.1 * (step_grid_M > 0.1);
				}
				if (event.key.code == Keyboard::M) {
					vector <int> changed_falgs = { number_color_style , (int)standart_vies , (int)show_marking_grid };
					GUI_menu(window, Arial, background, changed_falgs);
					number_color_style = changed_falgs[0];
					standart_vies = changed_falgs[1];
					show_marking_grid = changed_falgs[2];
					change_color_style(number_color_style, background);
				}
				if (event.key.code == Keyboard::S) {
					number_color_style++;
					if (number_color_style > 1)
						number_color_style = 0;
					change_color_style(number_color_style, background);
				}
				if (event.key.code == Keyboard::V)
					standart_vies = !standart_vies;
			}
			if (event.type == Event::MouseMoved) {
				window.setView(interfase_view);
				if (settings_button.isMouseOver(window))
					settings_in_focus = true;
				else
					settings_in_focus = false;
				//window.setView(pole_view);
			}

			if (event.type == sf::Event::MouseButtonReleased) {
				window.setView(pole_view);
				if (event.mouseButton.button == sf::Mouse::Left) {
					bool is_not_zarad = true;
					for (int i = 0; i < zarady.size(); i++) {
						double r = size_zar * abs(zarady[i].zarad);
						IntRect zar_calision(zarady[i].coords.x * convert_P_to_M - r,
							-zarady[i].coords.y * convert_P_to_M - r, r * 2, r * 2);
						Vector2i pos_mouse_coord = convert_F_to_I(window.mapPixelToCoords(Mouse::getPosition(window)));
						if (zar_calision.contains(pos_mouse_coord)) {
							number_change_zarad = i;
							is_not_zarad = false;
							break;
						}
					}
					if (is_not_zarad) {
						if (settings_in_focus) {
							vector <int> changed_falgs = { number_color_style , (int)standart_vies , (int)show_marking_grid };
							GUI_menu(window, Sans, background, changed_falgs);
							number_color_style = changed_falgs[0];
							standart_vies = changed_falgs[1];
							show_marking_grid = changed_falgs[2];

							window.setView(interfase_view);
							if (settings_button.isMouseOver(window))
								settings_in_focus = true;
							else
								settings_in_focus = false;
						}
						else {
							position_spot_info = convers_pixel_to_coordsF(window.mapPixelToCoords(Mouse::getPosition(window)),
								convert_P_to_M);
							show_spot_inform = !show_spot_inform;
						}
					}
				}
			}

			if (event.type == Event::MouseWheelScrolled)
				if (number_change_zarad < zarady.size()) {
					zarady[number_change_zarad].zarad += event.mouseWheelScroll.delta * 0.1;
					tensor_line_cadr_points = get_new_tensor_line_cadr(zarady);
				}
		}

		window.setView(pole_view);
		//отрисовка сетки
		if (show_marking_grid) {
			draw_marking_grid(window, pole_view, step_grid_M, convert_P_to_M);
			draw_coordinate_axes(window, pole_view, Arial, zoom, step_grid_M, convert_P_to_M);
		}
		// переход в слой (камеру) карты полей
		window.setView(pole_view);

		//отрисовка линий напряжённости
		for (int i = 0; i < tensor_line_cadr_points.size(); i++)
			draw_line(window, tensor_line_cadr_points[i],
				2, convert_P_to_M, zoom);

		//отрисовка зарядов
		draw_zarady(window, zarady, convert_P_to_M, size_zar);

		//отрисовка интерфейса
		fps.update();
		debug_info(window, TimesNewRoman, pole_view.getCenter(), pole_view.getSize(), fps,
			number_change_zarad);

		if (settings_in_focus && clock.getElapsedTime().asSeconds() >= 0.05) {
			settings_button.rotate(1);
			clock.restart();
		}
		window.setView(interfase_view);
		settings_button.drawTo(window);

		//
		if (show_spot_inform)
			draw_spot_information(window, pole_view, zoom,
				convert_coords_to_pixelsF(position_spot_info, convert_P_to_M),
				get_spot_information(position_spot_info, zarady, standart_vies), TimesNewRoman);

		// Отрисовка окна
		window.display();
	}
	return 0;
}

vector <vector < vector<string>>> buttons_title = {
	{{"Theme:"}, {"light","dark"}},
	{{"number format:"}, {"International\nSystem\nof Units", "scientific\nformat"}},
	{{"marking grid"},{"hide", "display.\nbinding\nis enabled"}}
};

void GUI_menu(RenderWindow& window, Font& font, Color& background_main_window, vector <int>& changed_falgs) {
	window.setView(window.getDefaultView());
	window.setKeyRepeatEnabled(true);

	Texture texture_exit_selected;
	if (!texture_exit_selected.loadFromFile("exit.png"))
	{
		cout << "петрович мы обосрались текстура крестика не грузиться" << endl;
	}
	Texture texture_FAQ_selected;
	if (!texture_FAQ_selected.loadFromFile("FAQ.png"))
	{
		// error...
	}
	Texture texture_exit_dark;
	if (!texture_exit_dark.loadFromFile("desaturate-exit.png"))
	{
		cout << "петрович мы обосрались текстура крестика не грузиться" << endl;
	}
	Texture texture_FAQ_dark;
	if (!texture_FAQ_dark.loadFromFile("desaturate-FAQ.png"))
	{
		// error...
	}
	Texture texture_exit_light;
	if (!texture_exit_light.loadFromFile("black-exit.png"))
	{
		cout << "петрович мы обосрались текстура крестика не грузиться" << endl;
	}
	Texture texture_FAQ_light;
	if (!texture_FAQ_light.loadFromFile("black-FAQ.png"))
	{
		// error...
	}
	

	vector <Button> buttons;
	Vector2f size_btn = { 200, 125 };
	for (int i = 0; i < buttons_title.size(); i++)
		buttons.push_back(Button("", size_btn, 30, sf::Color::Green, sf::Color::Black));
	for (int i = 0; i < buttons.size(); i++)
		buttons[i].setFont(font);
	float step_x = size_btn.x + 50;
	float step_y = size_btn.y + 50;
	for (int i = 0; i < buttons.size() / 2; i++) {
		buttons[i].setPosition({ step_x, 25 + step_y * i });
		buttons[i].setDescriptionString(buttons_title[i][0][0]);
		buttons[i].setTittleSring(buttons_title[i][1][changed_falgs[i]]);
	}
	for (int i = buttons.size() / 2; i < buttons.size(); i++) {
		buttons[i].setPosition({ step_x * 3,25 + step_y * (i - buttons.size() / 2) });
		buttons[i].setDescriptionString(buttons_title[i][0][0]);
		buttons[i].setTittleSring(buttons_title[i][1][changed_falgs[i]]);
	}

	Texture act_texture_exit;
	Texture act_texture_FAQ;

	Color text_color;
	Color button_in_focus;
	Color button_standard;
	Color background;
	if (changed_falgs[0]) {
		text_color = Color::White;
		button_in_focus = sf::Color::Magenta;
		button_standard = { 35, 134, 54 };
		act_texture_exit = texture_exit_dark;
		act_texture_FAQ = texture_FAQ_dark;
		background = Color::Black;
	}
	else {
		button_in_focus = { 254 , 190 ,214 };
		button_standard = Color::Green;
		text_color = Color::Black;
		act_texture_exit = texture_exit_light;
		act_texture_FAQ = texture_FAQ_light;
		background = { 189 , 248 , 255 };
	}
	for (int i = 0; i < buttons.size(); i++)
		buttons[i].setTextColor(text_color);

	Button exit_button(act_texture_exit, { 150, 150 });
	Button FAQ_button(act_texture_FAQ, { 150, 150 });
	exit_button.setPosition({ 1000,50 });
	FAQ_button.setPosition({ 1000,500 });
	//Main Loop:
	bool is_menu_not_closed = true;
	while (window.isOpen() && is_menu_not_closed) {

		sf::Event event;

		//Event Loop:
		while (window.pollEvent(event)) {
			// Пользователь нажал на «крестик» и хочет закрыть окно?
			if (event.type == Event::Closed)
				window.close();
			if (event.type == Event::KeyReleased)
				if (event.key.code == Keyboard::Escape)
					is_menu_not_closed = false;
			if (event.type == Event::MouseMoved) {
				for (int i = 0; i < buttons.size(); i++)
					if (buttons[i].isMouseOver(window))
						buttons[i].setBackColor(button_in_focus);
					else
						buttons[i].setBackColor(button_standard);

				if (exit_button.isMouseOver(window))
					exit_button.setTexture(texture_exit_selected);
				else
					exit_button.setTexture(act_texture_exit);

				if (FAQ_button.isMouseOver(window))
					FAQ_button.setTexture(texture_FAQ_selected);
				else
					FAQ_button.setTexture(act_texture_FAQ);
			}
			if (event.type == Event::MouseButtonReleased) {
				for (int i = 0; i < buttons.size(); i++)
					if (buttons[i].isMouseOver(window)) {
						changed_falgs[i]++;
						if (changed_falgs[i] == buttons_title[i][1].size())
							changed_falgs[i] = 0;
						buttons[i].setTittleSring(buttons_title[i][1][changed_falgs[i]]);

						switch (i)
						{
						case 0:
							if (changed_falgs[0]) {
								text_color = Color::White;
								button_in_focus = sf::Color::Magenta;
								button_standard = { 35, 134, 54 };
								act_texture_exit = texture_exit_dark;
								act_texture_FAQ = texture_FAQ_dark;
								background = Color::Black;
							}
							else {
								button_in_focus = { 254 , 190 ,214 };
								button_standard = Color::Green;
								text_color = Color::Black;
								act_texture_exit = texture_exit_light;
								act_texture_FAQ = texture_FAQ_light;
								background = { 189 , 248 , 255 };
							}
							for (int i = 0; i < buttons.size(); i++)
								buttons[i].setTextColor(text_color);

							change_color_style(changed_falgs[0], background_main_window);
							FAQ_button.setTexture(act_texture_FAQ);
							exit_button.setTexture(act_texture_exit);
							break;
						default:
							break;
						}
					}

				if (exit_button.isMouseOver(window))
					is_menu_not_closed = false;

				if (FAQ_button.isMouseOver(window)) {
					FAQ_list(window, texture_exit_selected, act_texture_exit, background_main_window,
					text_color, font);
					if (exit_button.isMouseOver(window))
						exit_button.setTexture(texture_exit_selected);
					else
						exit_button.setTexture(act_texture_exit);
					if (FAQ_button.isMouseOver(window))
						FAQ_button.setTexture(texture_FAQ_selected);
					else
						FAQ_button.setTexture(act_texture_FAQ);
				}
			}
		}
		window.clear(background);
		for (int i = 0; i < buttons.size(); i++)
			buttons[i].drawTo(window);
		exit_button.drawTo(window);
		FAQ_button.drawTo(window);
		window.display();
	}
}



void FAQ_list(RenderWindow& window, Texture & texture_exit_selected, Texture& act_texture_exit, 
	Color background, Color text_color, Font &font) {
	bool is_menu_not_closed = true;
	Button exit_button(act_texture_exit, { 150, 150 });
	exit_button.setPosition({ 1000,50 });

	Texture github_texture;
	if (!github_texture.loadFromFile("github.png"))
	{
		cout << "петрович мы обосрались текстура крестика не грузиться" << endl;
	}
	Button github_button(github_texture, { 150, 150 });
	github_button.setPosition({ 1000,450 });
	CircleShape shape(90.f, 300);
	shape.setFillColor(Color::White);
	shape.setPosition({ 984, 435 });

	wstring TEXT_FAQ = L"Это FAQ\n";
	TEXT_FAQ  = TEXT_FAQ + L"\nстрелочки - передвежение по полю\n" + L"S - смена темы\n" + L"pg up, pg down - зум\n" +
		L"+ (который =) и _ (который -) - изменение маштаба сетки\n" + L"G - включение отключение сетки и привязки (к сетке)\n" +
		L"Q - добавить заряд (в месте нахождения мышки\n" + L"ЛКМ - информация о точке поля\n";
	wstring TEXT_FAQ_created = L"created by pvh_cherpak\n";

	Text text_created;
	text_created.setString(TEXT_FAQ_created);
	text_created.setFont(font);
	text_created.setPosition(800, 620);
	text_created.setFillColor(text_color);
	Text txt;
	txt.setString(TEXT_FAQ);
	txt.setFont(font);
	txt.setPosition(100, 100);
	txt.setFillColor(text_color);
	
	if (github_button.isMouseOver(window))
		shape.setFillColor({1,188,186 });
	else
		shape.setFillColor(Color::White);
	while (window.isOpen() && is_menu_not_closed) {
		sf::Event event;
		//Event Loop:
		while (window.pollEvent(event)) {
			// Пользователь нажал на «крестик» и хочет закрыть окно?
			if (event.type == Event::Closed)
				window.close();
			if (event.type == Event::KeyReleased)
				if (event.key.code == Keyboard::Escape)
					is_menu_not_closed = false;
			if (event.type == Event::MouseMoved) {
				if (exit_button.isMouseOver(window))
					exit_button.setTexture(texture_exit_selected);
				else
					exit_button.setTexture(act_texture_exit);

				if (github_button.isMouseOver(window))
					shape.setFillColor({ 1,188,186 });
				else
					shape.setFillColor(Color::White);
			}
			if (event.type == Event::MouseButtonReleased) {
				if (exit_button.isMouseOver(window))
					is_menu_not_closed = false;
				if (github_button.isMouseOver(window))
					system("start https://github.com/pvh-cherpak");
			}
		}
		window.clear(background);
		window.draw(shape);
		exit_button.drawTo(window);
		window.draw(txt);
		github_button.drawTo(window);
		window.draw(text_created);
		window.display();
	}
	
}