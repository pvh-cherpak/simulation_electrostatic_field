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
	Vector2f coords;
	double zarad ;
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

vector <vector < vector<wstring>>> buttons_title_english = {
	{{L"Theme:"}, {L"light",L"dark"}},
	{{L"number format:"}, {L"International\nSystem\nof Units", L"scientific\nformat"}},
	{{L"marking grid"},{L"hide", L"display.\nbinding\nis enabled"}},
	{{L"language: "}, {L"russian",L"english"}}
};

vector <vector < vector<wstring>>> buttons_title_russian = {
	{{L"����:"}, {L"�������",L"�����"}},
	{{L"������������� �����:"}, {L"�������\n��", L"�������\n�������������"}},
	{{L"��� ���������"},{L"������", L"����������.\n�������� � �����\n��������"}},
	{{L"����: "}, {L"�������", L"����������"}}
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