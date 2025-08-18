#pragma once

#include <SFML/Graphics.hpp>

namespace stu {

	class NumberBox {
	private:

		sf::RectangleShape box;

		sf::Text text;

		sf::Font default_font;

		std::string user_text;

		bool isActive;

		bool isVisible;

		sf::Text label;


	public:

		NumberBox();

		NumberBox(float width, float height, sf::Font font, std::string title = "Input");

		std::string getText();

		bool isBoxPressed(sf::RenderWindow &window, sf::Event &event);

		bool isTyping(sf::Event& event);

		std::string getTypedText(sf::Event &event);

		void setSize(float width, float height);

		void setLabelText(sf::String text);

		void setFont(sf::Font font);

		void setBoxColor(sf::Color box_color);

		void setTextColor(sf::Color color);
		
		void setLabelTextColor(sf::Color color);

		void setLabelCharSize(int character_size);

		void setInputCharSize(int character_size);

		void setPosition(float x, float y);

		void setActive(bool state);

		void setVisible(bool state);

		bool isactive();

		bool isvisible();

		void reset();

		void draw(sf::RenderWindow& window);

	};
	
}