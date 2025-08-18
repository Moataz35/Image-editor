#include "CropRectangle.h"
#include <SFML/Graphics.hpp>
#include <iostream>

CropRectangle::CropRectangle() {
	shape.setSize(sf::Vector2f(100, 100));
	shape.setPosition(200, 200);
	shape.setFillColor(sf::Color::Transparent);
	shape.setOutlineThickness(3);
	shape.setOutlineColor(sf::Color::Black);

	image_to_crop = nullptr;


	global_press_x = 0;
	global_press_y = 0;
	local_press_x = 0;
	local_press_y = 0;

	hold_corner_number = 0;

	from_corner_x = 0;
	from_corner_y = 0;

	isVisible = false;
	isPressed = false;
	isResizing = false;
}

CropRectangle::CropRectangle(stu::Texture* image) {
	image_to_crop = image;
	
	shape.setSize(
		image_to_crop->getSize()
	);
	shape.setPosition(
		image_to_crop->getPosition()
	);
	shape.setFillColor(sf::Color::Transparent);
	shape.setOutlineThickness(3);
	shape.setOutlineColor(sf::Color::Black);


	
	global_press_x = 0;
	global_press_y = 0;
	local_press_x = 0;
	local_press_y = 0;

	hold_corner_number = 0;

	from_corner_x = 0;
	from_corner_y = 0;

	isVisible = false;
	isPressed = false;
	isResizing = false;
}

void CropRectangle::setTexture(stu::Texture* image) {
	image_to_crop = image;
	shape.setSize(
		image_to_crop->getSize()
	);
	shape.setPosition(
		image_to_crop->getPosition()
	);

}

void CropRectangle::setVisible(bool state) {
	// Before we set the rectangle visible we should update it with the latest info from the texture
	shape.setSize(
		image_to_crop->getSize()
	);
	shape.setPosition(
		image_to_crop->getPosition()
	);
	isVisible = state;
}

sf::Vector2f CropRectangle::getNewImageBegin() {
	sf::Vector2f image_begin = shape.getPosition();
	image_begin.x -= image_to_crop->getPosition().x;
	image_begin.y -= image_to_crop->getPosition().y;
	return image_begin;
}

sf::Vector2f CropRectangle::getNewImageSize() {
	return shape.getSize();
}

bool CropRectangle::isRectanglePressed(sf::RenderWindow& window, sf::Event& event) {
	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
		float mouse_x = sf::Mouse::getPosition(window).x;
		float mouse_y = sf::Mouse::getPosition(window).y;
		if (shape.getGlobalBounds().contains(mouse_x, mouse_y)) {
			isPressed = true;
			// We need the point that the mouse was on when the rectangle is pressed to move with the mouse
			global_press_x = mouse_x;
			global_press_y = mouse_y;
			local_press_x = mouse_x - shape.getPosition().x;
			local_press_y = mouse_y - shape.getPosition().y;

			if (isFromCorner(local_press_x, local_press_y)) {
				shape.setOutlineColor(sf::Color::White);
			}
			return true;
		}
		// We will not change the crop rectangle origin because it will corrupt the mouse position relative to the rectangle calculation
	}
	return false;
}

bool CropRectangle::isRectangleReleased(sf::RenderWindow& window, sf::Event& event) {
	if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
		isPressed = false;
		if (isResizing) {
			changeSize(window);
			shape.setOutlineColor(sf::Color::Black);
			isResizing = false;
		}
		return true;
	}
	return false;
}

bool CropRectangle::isRectangleMoved(sf::RenderWindow& window, sf::Event& event) {
	if (image_to_crop == nullptr) return false;

	if (event.type == sf::Event::MouseMoved && isPressed) {

		if (isFromCorner(local_press_x, local_press_y)) {
			// If we grap the rectangle from one of its corner then we want to resize it
			isResizing = true;
		}
		else {
			move(window);
		}
		return true;
	}
	return false;
}

bool CropRectangle::isMovedWithKey(sf::RenderWindow& window, sf::Event& event) {
	if (image_to_crop == nullptr) return false;

	if (event.type == sf::Event::KeyPressed) {
		switch (event.key.code) {
		case sf::Keyboard::Left:
			if (shape.getPosition().x >= image_to_crop->getPosition().x + 5.0f) {
				shape.move(-5.0f, 0);
			}
			break;
		case sf::Keyboard::Right:
			if (shape.getPosition().x + shape.getSize().x <= image_to_crop->getPosition().x + image_to_crop->getSize().x - 5.0f) {
				shape.move(5.0f, 0);
			}
			break;
		case sf::Keyboard::Up:
			if (shape.getPosition().y >= image_to_crop->getPosition().y + 5.0f) {
				shape.move(0, -5.0f);
			}
			break;
		case sf::Keyboard::Down:
			if (shape.getPosition().y + shape.getSize().y <= image_to_crop->getPosition().y + image_to_crop->getSize().y - 5.0f) {
				shape.move(0, 5.0f);
			}
			break;
		default:
			break;
		}
		return true;
	}
	return false;
}

bool CropRectangle::isFromCorner(float x, float y, float corner) {
	float shape_width = shape.getSize().x;
	float shape_height = shape.getSize().y;
	bool first_x = x >= 0 && x < corner;
	bool first_y = y >= 0 && y < corner;
	bool last_x = x >= shape_width - corner && x < shape_width;
	bool last_y = y >= shape_height - corner && y < shape_height;

	// First 5px from x and First 5px from y (Top left)
	bool isCorner1 = first_x && first_y;
	// last 5px from x and First 5px from y (Top right)
	bool isCorner2 = last_x && first_y;
	// last 5px from x and last 5px from y (bottom right)
	bool isCorner3 = last_x && last_y;
	// First 5px from x and last 5px from y (bottom left)
	bool isCorner4 = first_x && last_y;

	if (isCorner1) {
		hold_corner_number = 1;
		from_corner_x = local_press_x;
		from_corner_y = local_press_y;
		return true;
	}
	if (isCorner2) {
		hold_corner_number = 2;
		from_corner_x = shape_width - local_press_x;
		from_corner_y = local_press_y;
		return true;
	}
	if (isCorner3) {
		hold_corner_number = 3;
		from_corner_x = shape_width - local_press_x;
		from_corner_y = shape_height - local_press_y;
		return true;
	}
	if (isCorner4) {
		hold_corner_number = 4;
		from_corner_x = local_press_x;
		from_corner_y = shape_height - local_press_y;
		return true;
	}
	return false;
}

void CropRectangle::changeSize(sf::RenderWindow& window) {
	if (image_to_crop == nullptr) return;
	// The difference between the holded corner and the mouse new position will be added or subtracted from the size

	sf::Vector2f mouse_position(sf::Mouse::getPosition(window));
	sf::Vector2f image_begin(image_to_crop->getPosition());
	sf::Vector2f image_end;
	image_end.x = image_begin.x + image_to_crop->getSize().x;
	image_end.y = image_begin.y + image_to_crop->getSize().y;

	if (mouse_position.x < image_begin.x) {
		mouse_position.x = image_begin.x;
	}
	else if (mouse_position.x > image_end.x) {
		mouse_position.x = image_end.x;
	}

	if (mouse_position.y < image_begin.y) {
		mouse_position.y = image_begin.y;
	}
	else if (mouse_position.y > image_end.y) {
		mouse_position.y = image_end.y;
	}

	float width = shape.getSize().x;
	float height = shape.getSize().y;

	float width_difference = abs(mouse_position.x - global_press_x);
	float height_difference = abs(mouse_position.y - global_press_y);


	// If the mouse enterd the rectangle then we will make the rectangle smaller and bigger otherwise
	if (shape.getGlobalBounds().contains(mouse_position.x, mouse_position.y)) {
		width -= width_difference;
		height -= height_difference;
	}
	else {
		width += width_difference;
		height += height_difference;
	}

	// There is a minimum size for the rectangle
	width = std::max(width, 40.0f);
	height = std::max(height, 40.0f);

	// The maximum size is the image size
	width = std::min(width, image_to_crop->getSize().x);
	height = std::min(height, image_to_crop->getSize().y);

	shape.setSize(sf::Vector2f(width, height));

	// Change the position of the holded corner with the mouse new position
	changePosition(window, mouse_position);
}

void CropRectangle::changePosition(sf::RenderWindow& window, sf::Vector2f mouse_position) {
	if (image_to_crop == nullptr) return;
	// The holded corner will be at the mouse new position
	// First we will move the left corner to that position
	// then according to the corner number we will put that corner at the new position by subtracting the true dimensions
	// We didn't exactly hold the corner but a point near to it so we need to subract the distance between the corner and that point
	sf::Vector2f shape_begin(mouse_position);

	if (hold_corner_number == 2) {
		shape_begin.x -= (shape.getSize().x - from_corner_x);
	}
	else if (hold_corner_number == 3) {
		shape_begin.x -= (shape.getSize().x - from_corner_x);
		shape_begin.y -= (shape.getSize().y - from_corner_y);
	}
	else if (hold_corner_number == 4) {
		shape_begin.y -= (shape.getSize().y - from_corner_y);
	}
	
	// Keep the rectangle in the image bounds
	sf::Vector2f image_begin(image_to_crop->getPosition());
	sf::Vector2f image_end;
	image_end.x = image_begin.x + image_to_crop->getSize().x;
	image_end.y = image_begin.y + image_to_crop->getSize().y;

	sf::Vector2f shape_end;
	shape_end.x = shape_begin.x + shape.getSize().x;
	shape_end.y = shape_begin.y + shape.getSize().y;

	if (shape_begin.x < image_begin.x) {
		shape_begin.x = image_begin.x;
	}
	if (shape_begin.y < image_begin.y) {
		shape_begin.y = image_begin.y;
	}
	if (shape_end.x > image_end.x) {
		shape_begin.x = image_end.x - shape.getSize().x;
	}
	if (shape_end.y > image_end.y) {
		shape_begin.y = image_end.y - shape.getSize().y;
	}
	

	shape.setPosition(shape_begin.x, shape_begin.y);
}

void CropRectangle::move(sf::RenderWindow& window) {
	if (image_to_crop == nullptr) return;
	// We will move the crop rectangle top left to be at the same distance from the pressed point with its new position
	// 1. The top left corner at the mouse new position
	// 2. subtract the local pressed point to make it on the mouse new position not the top left corner
	// The rectangle can only move inside the image
	// Move the rectangle even if the mouse is outside the image but don't move the rectangle outside the image
	sf::Vector2f shape_begin;
	shape_begin.x = sf::Mouse::getPosition(window).x - local_press_x; 
	shape_begin.y = sf::Mouse::getPosition(window).y - local_press_y;

	sf::Vector2f shape_end;
	shape_end.x = shape_begin.x + shape.getSize().x;
	shape_end.y = shape_begin.y + shape.getSize().y;

	sf::Vector2f image_begin(image_to_crop->getPosition());

	sf::Vector2f image_end;
	image_end.x = image_begin.x + image_to_crop->getSize().x;
	image_end.y = image_begin.y + image_to_crop->getSize().y;

	// Check if it's not in bounds and force it to be in bounds
	if (shape_begin.x < image_begin.x) {
		shape_begin.x = image_begin.x;
	}
	if (shape_begin.y < image_begin.y) {
		shape_begin.y = image_begin.y;
	}
	if (shape_end.x > image_end.x) {
		shape_begin.x = image_end.x - shape.getSize().x;
	}
	if (shape_end.y > image_end.y) {
		shape_begin.y = image_end.y - shape.getSize().y;
	}
	shape.setPosition(shape_begin.x, shape_begin.y);
}

void CropRectangle::draw(sf::RenderWindow& window) {
	if (isVisible && image_to_crop != nullptr) {
		window.draw(shape);
	}
}