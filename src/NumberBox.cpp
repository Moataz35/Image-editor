#include "NumberBox.h"
#include <SFML/Graphics.hpp>
#include <iostream>

stu::NumberBox::NumberBox() {
	box.setSize(sf::Vector2f(70.0f, 40.0f));
	box.setFillColor(sf::Color::White);

	user_text = "";

	text.setCharacterSize(20);
	text.setFillColor(sf::Color::Black);
	text.setPosition(box.getPosition().x + 1, box.getPosition().y + 1);

	
	label.setCharacterSize(16);
	label.setFillColor(sf::Color::White);
	float label_height = label.getLocalBounds().height + label.getLocalBounds().top;
	label.setPosition(box.getPosition().x, box.getPosition().y - label_height);

	isActive = false;
	isVisible = false;
}

stu::NumberBox::NumberBox(float width, float height, sf::Font font, std::string title) {
	box.setSize(sf::Vector2f(width, height));
	box.setFillColor(sf::Color::White);
	box.setOutlineThickness(1.0f);
	box.setOutlineColor(sf::Color::Black);

	user_text = "";

	default_font = font;

	text.setFont(default_font);
	text.setString(user_text);
	text.setCharacterSize(20);
	text.setFillColor(sf::Color::Black);
	text.setPosition(box.getPosition().x + 1, box.getPosition().y + 1);

	label.setFont(default_font);
	label.setString(title);
	label.setCharacterSize(16);
	label.setFillColor(sf::Color::White); 
	float label_height = label.getLocalBounds().height + label.getLocalBounds().top;
	label.setPosition(box.getPosition().x, box.getPosition().y - label_height);

	isActive = false;
	isVisible = false;
}

std::string stu::NumberBox::getText() {
	return user_text;
}

bool stu::NumberBox::isBoxPressed(sf::RenderWindow& window, sf::Event& event) {
	if (!isVisible) return false;

	// If the mouse left button is pressed and its position was on the button then it has been pressed
	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
		int mouse_x = sf::Mouse::getPosition(window).x;
		int mouse_y = sf::Mouse::getPosition(window).y;
		// Global Bounds -> according to the window
		// Local Bounds -> according to the shape itself
		if (box.getGlobalBounds().contains(mouse_x, mouse_y)) {
			return true;
		}
	}

	return false;
}

bool stu::NumberBox::isTyping(sf::Event& event) {
	if (isActive && event.type == sf::Event::TextEntered) {
		return true;
	}
	return false;
}

std::string stu::NumberBox::getTypedText(sf::Event& event) {

	// TextEntered event detect if one of the characters is pressed and returns the ascii value of it
	char entered_char = event.text.unicode < 128 ? static_cast<char>(event.text.unicode) : '!';

	bool isFinishTyping = false;

	if (entered_char >= '0' && entered_char <= '9' && user_text.length() < 4) {
		user_text += entered_char;
	}
	else if (entered_char == 8) { // Backspace key

		if (user_text.length() > 0) {
			user_text.pop_back();
		}

	}
	else if (entered_char == 13) { // Enter key

		if (user_text.length() > 0) {
			isFinishTyping = true;
		}

	}

	// If the user finish typing return the enterd text
	if (isFinishTyping) {
		text.setString("");
		isActive = false;
		isVisible = false;
		return user_text;
	}
	text.setString(user_text);
	return "";
}


void stu::NumberBox::setSize(float width, float height) {
	box.setSize(sf::Vector2f(width, height));
}

void stu::NumberBox::setLabelText(sf::String text) {
	label.setString(text);
}

void stu::NumberBox::setFont(sf::Font font) {
	default_font = font;
	text.setFont(default_font);
	text.setString("");
	label.setFont(default_font);
	label.setString("Input");
}

void stu::NumberBox::setBoxColor(sf::Color box_color) {
	box.setFillColor(box_color);
}

void stu::NumberBox::setTextColor(sf::Color color) {
	text.setFillColor(color);
}

void stu::NumberBox::setLabelTextColor(sf::Color color) {
	label.setFillColor(color);
}

void stu::NumberBox::setLabelCharSize(int character_size) {
	text.setCharacterSize(character_size);
}

void stu::NumberBox::setInputCharSize(int character_size) {
	label.setCharacterSize(character_size);
}

void stu::NumberBox::setPosition(float x, float y) {
	box.setPosition(sf::Vector2f(x, y));
	text.setPosition(x + 1, y + 1);
	float label_height = label.getLocalBounds().height + label.getLocalBounds().top;
	label.setPosition(box.getPosition().x, box.getPosition().y - label_height);
}

void stu::NumberBox::setActive(bool state = false) {
	isActive = state;
}

void stu::NumberBox::setVisible(bool state = false) {
	isVisible = state;
}

bool stu::NumberBox::isactive() {
	return isActive;
}

bool stu::NumberBox::isvisible() {
	return isVisible;
}

void stu::NumberBox::reset() {
	user_text.clear();
	text.setString("");
}

void stu::NumberBox::draw(sf::RenderWindow& window) {
	if (isVisible) {
		window.draw(box);
		window.draw(text);
		window.draw(label);
	}
}