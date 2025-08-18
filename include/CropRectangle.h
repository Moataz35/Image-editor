#pragma once

#include "Texture.h"
#include <SFML/Graphics.hpp>

class CropRectangle {
private:
	sf::RectangleShape shape;
	stu::Texture* image_to_crop;
	// The holded corner information:
	// The point that we hold the rectangle from relative to the window origin
	float global_press_x;
	float global_press_y;
	// The corner origin is at the top left of the window
	// We want to calculate it from the begin of the shape
	float local_press_x;
	float local_press_y;
	int hold_corner_number;
	// To adjust the position after resizing we want to know how far the pressed point is from the nearest corner
	float from_corner_x;
	float from_corner_y;

	bool isVisible;
	bool isPressed;
	bool isResizing;

	// Check if the pressed point is from one of the corners (one corner is 15.0f*15.0f size by default)
	bool isFromCorner(float x, float y, float corner = 15.0f);
	void changeSize(sf::RenderWindow& window);
	void changePosition(sf::RenderWindow& window, sf::Vector2f mouse_position); // Take the mouse position after the changeSize() edit its position
	void move(sf::RenderWindow& window);
public:
	CropRectangle();
	CropRectangle(stu::Texture* image);

	void setTexture(stu::Texture* image);
	void setVisible(bool state);
	sf::Vector2f getNewImageBegin();
	sf::Vector2f getNewImageSize();
	bool isRectanglePressed(sf::RenderWindow& window, sf::Event& event);
	bool isRectangleReleased(sf::RenderWindow& window, sf::Event& event);

	// Move the rectangle by holding it with the mouse
	bool isRectangleMoved(sf::RenderWindow& window, sf::Event& event);

	// Another option to move the rectangle with the keyboard
	bool isMovedWithKey(sf::RenderWindow& window, sf::Event& event);

	void draw(sf::RenderWindow& window);
};