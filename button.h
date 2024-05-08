// Made by Ben Bollinger

#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

class Button {
public:
	Button(std::string btnText, sf::Vector2f buttonSize, int charSize, sf::Color bgColor, sf::Color textColor) {
		button.setSize(buttonSize);
		button.setFillColor(bgColor);

		// Get these for later use:
		btnWidth = buttonSize.x;
		btnHeight = buttonSize.y;

		btn.height = buttonSize.y;
		btn.width = buttonSize.x;

		text.setString(btnText);
		text.setCharacterSize(charSize);
		description.setCharacterSize(charSize);
		text.setFillColor(textColor);
		description.setFillColor(textColor);
	}

	Button(Texture &texture, sf::Vector2f buttonSize) {
		button.setTexture(&texture);
		button.setSize(buttonSize);

		btn.height = buttonSize.x;
		btn.width = buttonSize.y;
		// Get these for later use:
		btnWidth = buttonSize.x;
		btnHeight = buttonSize.y;

	}

	// Pass font by reference:
	void setFont(sf::Font& fonts) {
		text.setFont(fonts);
		description.setFont(fonts);
	}

	void setBackColor(sf::Color color) {
		button.setFillColor(color);
	}

	void setTextColor(sf::Color color) {
		text.setFillColor(color);
		description.setFillColor(color);
	}

	void setPosition(sf::Vector2f point) {
		button.setPosition(point);
		btn.left = point.x;
		btn.top = point.y;

		// Center text on button:
		float xPos = (button.getPosition().x + btnWidth / 2) - (text.getLocalBounds().width / 2);
		float yPos = (button.getPosition().y + btnHeight / 2.3) - (text.getLocalBounds().height / 2);
		text.setPosition(xPos, yPos);
	}

	void drawTo(sf::RenderWindow& window) {
		window.draw(button);
		window.draw(text);
		window.draw(description);
	}

	void setTittleSring(string new_tittle_string) {
		text.setString(new_tittle_string);
		float xPos = (button.getPosition().x + btnWidth / 2) - (text.getLocalBounds().width / 2);
		float yPos = (button.getPosition().y + btnHeight / 2.3) - (text.getLocalBounds().height / 2);
		text.setPosition(xPos, yPos);
	}

	void setTittleSring(wstring new_tittle_string) {
		text.setString(new_tittle_string);
		float xPos = (button.getPosition().x + btnWidth / 2) - (text.getLocalBounds().width / 2);
		float yPos = (button.getPosition().y + btnHeight / 2.3) - (text.getLocalBounds().height / 2);
		text.setPosition(xPos, yPos);
	}

	string getTittleSring() {
		return text.getString();
	}

	void setTexture(Texture &texture) {
		button.setTexture(&texture);
	}

	void setDescriptionString(string new_description_string) {
		description.setString(new_description_string);
		float xPos = (button.getPosition().x) - (description.getLocalBounds().width) - 50;
		float yPos = (button.getPosition().y + btnHeight / 2.2) - (description.getLocalBounds().height / 2);
		description.setPosition(xPos, yPos);
	}

	void setDescriptionString(wstring new_description_string) {
		description.setString(new_description_string);
		float xPos = (button.getPosition().x) - (description.getLocalBounds().width) - 50;
		float yPos = (button.getPosition().y + btnHeight / 2.2) - (description.getLocalBounds().height / 2);
		description.setPosition(xPos, yPos);
	}

	// Check if the mouse is within the bounds of the button:
	bool isMouseOver(sf::RenderWindow& window) {
		Vector2f v  = window.mapPixelToCoords( sf::Mouse::getPosition(window));
		int mouseX = v.x;
		int mouseY = v.y;

		if (btn.contains({ mouseX , mouseY })) {
			return true;
		}
		return false;
	}
	void set_center_in_center() {
		button.setOrigin(btnWidth / 2.0, btnHeight / 2.0);
		btn.top -= btn.height / 2;
		btn.left -= btn.width / 2;
	}
	void rotate(double angle) {
		button.rotate(angle);
	}
	
private:
	sf::RectangleShape button;
	sf::Text text;
	sf::Text description;
	IntRect btn;
	int btnWidth;
	int btnHeight;
};