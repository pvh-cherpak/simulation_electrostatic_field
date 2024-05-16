#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;
using namespace sf;

struct point
{
	double x, y;
};

struct Zarad
{
	Vector2f coords = { 0,0 };
	double zarad = 1e-5;
	bool is_locked = false;
	Vector2f speed = { 0, 0};
	Vector2f acceleration = { 0 , 0};
	double mass = 1;
};

class FPS
{
public:
	FPS() : mFrame(0), mFps(0) {}


	const unsigned int getFPS() const { return mFps; }
private:
	unsigned int mFrame;
	unsigned int mFps;
	sf::Clock mClock;

public:
	void update()
	{
		if (mClock.getElapsedTime().asSeconds() >= 1.f)
		{
			mFps = mFrame;
			mFrame = 0;
			mClock.restart();
		}

		++mFrame;
	}
};
double zoom = 1;
vector <vector < vector<wstring>>> buttons_title_english = {
	{{L"Theme:"}, {L"light",L"dark",L"field\nintensity"}},
	{{L"number format:"}, {L"International\nSystem\nof Units", L"scientific\nformat"}},
	{{L"marking grid"},{L"hide", L"display.\nbinding\nis enabled"}},
	{{L"language: "}, {L"russian",L"english"}},
	{{L"field intensyty line"},{L"hide", L"show"}}
};

vector <vector < vector<wstring>>> buttons_title_russian = {
	{{L"����:"}, {L"�������",L"�����",L"������������\n����"}},
	{{L"������������� �����:"}, {L"�������\n��", L"�������\n�������������"}},
	{{L"��� ���������"},{L"������", L"����������.\n�������� � �����\n��������"}},
	{{L"����: "}, {L"�������", L"����������"}},
	{{L"����� ������������"},{L"������",L"������������"}}
};

vector <wstring> spot_info_english = {
	L"electrostatic field strength: ", L"Volt/meter",
	L"electrostatic field potential: ", L"Volt"
};

vector <wstring> spot_info_russian = {
	L"������������ ������������������� ����: ", L"�����/����",
	L"��������� ������������������� ����: ", L"�����"
};

vector <wstring> prefix_english = { L"pico", L"nano", L"micro", L"milli",L"",L"kilo",L"mega",L"giga",L"tera" };
vector <wstring> prefix_russian = { L"����", L"����", L"�����", L"����",L"",L"����",L"����",L"����",L"����" };

vector <vector < vector<wstring>>> buttons_title;

vector <wstring> charge_ru = { L"�����: " , L"�����", L"���: ", L"��", L"��������� ������: ", L"������������", L"��������", L"������ ��������: ", L"�/�"};
vector <wstring> charge_en = { L"Charge: " , L"Columb", L"Weight: ", L"Kg", L"state of charge: ", L"fixed", L"free", L"velocity modulus: ", L"M/s" };
vector <wstring> charge_text;