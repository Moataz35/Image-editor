#ifndef MyTexture_h
#define MyTexture_h

#include "Image.h"
#include <SFML/Graphics.hpp>
#include <string>

namespace stu {
	// An image that can be drawn on a window
	class Texture : public stu::Image {
	protected:
		sf::Texture drawable_image;
		sf::RectangleShape shape;
		std::string imageFile;
	public:
		Texture();
		Texture(std::string fileName);
		Texture(const Texture& tex);
		Texture& operator=(const Texture& tex);
		bool loadFromFile(std::string fileName);
		void updateTexture();
		void setPosition(float x, float y);
		void setSize(float width, float height);
		sf::Vector2f getSize();
		sf::Vector2f getPosition();
		std::string getImageName();
		void draw(sf::RenderWindow& window);
	};
}

#endif