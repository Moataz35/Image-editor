#include "Texture.h"
#include <SFML/Graphics.hpp>
#include <iostream>


stu::Texture::Texture() : Image(), drawable_image(), shape()  {
	
}

stu::Texture::Texture(std::string fileName) : Image(fileName) {
	loadFromFile(fileName);
}

stu::Texture::Texture(const stu::Texture& tex) : Image(tex) {
	this->drawable_image = tex.drawable_image;
	this->shape = tex.shape;
	this->imageFile = tex.imageFullname;
	//loadFromFile(imageFile);

	if (!drawable_image.loadFromFile(imageFile)) {
		std::cerr << "stu::Texture::Copy constructor: Failed to load the image!\n";
		
	}

	shape.setTexture(&drawable_image, true);

	int img_width = drawable_image.getSize().x;
	int img_height = drawable_image.getSize().y;

	// Edit the rectangle size which hold the texturn because the difference in size between images
	shape.setTextureRect(sf::IntRect(0, 0, img_width, img_height));
	shape.setSize(sf::Vector2f(img_width, img_height));
}

stu::Texture& stu::Texture::operator=(const stu::Texture& tex) {
	this->Image::operator=(tex);

	this->drawable_image = tex.drawable_image;
	this->shape = tex.shape;
	this->imageFile = tex.imageFullname;
	loadFromFile(imageFile);
	
	return *this;
}

bool stu::Texture::loadFromFile(std::string fileName) {
	// We should separate the loadFromFile for updating the image and loading to get the pixels

	if (!drawable_image.loadFromFile(fileName)) {
		std::cerr << "stu::Texture::loadFromFile(): Failed to load the image!\n";
		return false;
	}

	stu::Image::get_image(fileName);

	imageFile = fileName;
	shape.setTexture(&drawable_image, true);

	int img_width = drawable_image.getSize().x;
	int img_height = drawable_image.getSize().y;

	// Edit the rectangle size which hold the texturn because the difference in size between images
	shape.setTextureRect(sf::IntRect(0, 0, img_width, img_height));
	shape.setSize(sf::Vector2f(img_width, img_height));
	return true;
}

void stu::Texture::setPosition(float x, float y) {
	shape.setPosition(x, y);
	
}

void stu::Texture::setSize(float width, float height) {
	shape.setSize(sf::Vector2f(width, height));
}

sf::Vector2f stu::Texture::getSize() {
	return shape.getSize();
}

sf::Vector2f stu::Texture::getPosition() {
	return shape.getPosition();
}

std::string stu::Texture::getImageName() {
	return imageFullname;
}

void stu::Texture::draw(sf::RenderWindow& window) {
	//updateTexture();
	window.draw(shape);
}


void stu::Texture::updateTexture() {
	// Save the current pixels to the image
	saveChanges();
	// Load the new image after editing
	loadFromFile(imageFullname);
}