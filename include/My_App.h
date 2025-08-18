#pragma once

#include "Button.h"
#include "CropRectangle.h"
#include "Constants.h"
#include "NumberBox.h"
#include "Texture.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <stack>

class My_App {
private:
	sf::RenderWindow window;
	sf::Font arial_font;
	stu::Texture original_image;
	std::stack<stu::Texture> list_of_changes;
	std::stack<stu::Texture> changes_to_redo;
	std::vector<sf::String> filter_names;
	std::vector<Button> button_grid;
	Button undo;
	Button redo;
	bool isFilterDone;
	bool isUploadedImage;

	static constexpr float IMAGE_CENTER_X = 420.0f;
	static constexpr float IMAGE_CENTER_Y = 400.0f;
	// The message box possible answers
	static constexpr int YES = 1;
	static constexpr int NO = 2;
	static constexpr int CANCEL = 0;

	// For each button:
	// initializeButtons();
	// buttonGridEvent(sf::Event& main_event); If it's one of the grid buttons
	// eventHandling(sf::Event& main_event);
	// draw(sf::RenderWindow& window);

	Button open_button;
	Button save_button;
	Button save_as_button;
	Button reset_button;

	// Merge filter components
	Button merge_horizontal;
	Button merge_vertical;

	// Flip filter components
	Button flip_H;
	Button flip_V;

	// Rotate filter components
	Button rotate90;
	Button rotate180;
	Button rotate270;

	// Brightness filter components
	Button brightness_up;
	Button brightness_down;

	// Resize filter components
	stu::NumberBox width_box;
	stu::NumberBox height_box;
	std::string resizing_width;
	std::string resizing_height;

	// Crop filter components
	CropRectangle croppingRec;
	Button ok;
	Button cancel;

	// A function to initialize the buttons in the filter buttons grid
	void initializeGrid();
	// Initialize any other button in the app by changing the size, position, color and text
	void initializeButtons();
	// Initialize the number boxes of the resize filter by changing the size and position and the color
	void initializeNumberBoxes();

	/*
	This changes the image position to put it at the center of its space and change the size 
	if its bigger than the allowed size 800x600
	*/
	void adjustToFit();

	void eventHandling(sf::Event& main_event);
	bool isButtonGridEvent(sf::Event& main_event);
	bool isCroppingEvent(sf::RenderWindow& window, sf::Event& main_event);
	bool isResizingEvent(sf::Event& main_event);
	int loseChangesWarning();

	// Draw the app content
	void draw(sf::RenderWindow& window);
public:
	My_App();
	void run();
};

