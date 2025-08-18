#pragma once

#include <SFML/Graphics.hpp>

class Button {
	// Consists of Rectangle shape in the center of it a text
	/*
	It should contain:
	A function to check if it has been pressed and another one for release
	Center the text in the button
	Set position function
	
	Resizing functions:
	2.Resize the rectangle
	3.Resize the text
	
	Change the background color
	Change the text color
	Draw function
	Optional: Effects
	*/
	sf::Font main_font;
	sf::Text name;
	sf::RectangleShape shape;
	bool isVisible;
	bool pressed;
	bool released;
	sf::Clock after_press;
public:
	Button();

	Button(sf::Font font, sf::String button_text);

	void styleSheet(
		float width = 100.0f,
		float height = 100.0f,
		float x = 0.0f,
		float y = 0.0f,
		sf::String font_file_name = "Data/Arial.ttf",
		sf::String text = "Push",
		int char_size = 20,
		sf::Color background_color = sf::Color::Black,
		sf::Color text_color = sf::Color::White,
		bool state = true
	);

	void initializeButton(float width, float height, float x = 0, float y = 0, sf::Color background_color = sf::Color::Black);

	void changeTextSettings(sf::Font font, sf::String text, sf::Color text_color = sf::Color::White);

	bool isButtonPressed(sf::RenderWindow& window, sf::Event& event);

	bool isButtonReleased(sf::RenderWindow& window, sf::Event& event);

	bool isvisible();

	void setVisible(bool state);

	void centerText();

	void setFont(sf::String font_file);

	void setFont(sf::Font font_file);

	void setText(sf::String text);

	void setPosition(float x, float y);

	void setSize(float new_width, float new_height);

	void setBackgroundColor(sf::Color background);

	void setTextColor(sf::Color text_color);

	void setFrame(float thickness, sf::Color frame_color);

	sf::Vector2f getPosition();

	sf::Vector2f getSize();

	void applyEffect();

	void removeEffect();

	void draw(sf::RenderWindow& window);

};

