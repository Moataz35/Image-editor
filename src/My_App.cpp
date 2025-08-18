#include "My_App.h"
#include "tinyfiledialogs.h"

My_App::My_App()  {
	window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Image-processor", sf::Style::Close | sf::Style::Titlebar);
	window.setFramerateLimit(20);

	arial_font.loadFromFile("Data/Arial.ttf");

	original_image.loadFromFile("Data/placeholder.png");

	initializeGrid();

	initializeButtons();

	initializeNumberBoxes();

	croppingRec.setTexture(&original_image);

	adjustToFit();

	isFilterDone = true;
	isUploadedImage = false;
}

void My_App::initializeGrid() {
	filter_names = {
		"Grayscale", "B and W", "Negative", "Edge detection", "Merge", "Flip", "Rotate",
		"Brightness", "Crop", "Add frame", "Resize", "Blur"
	};

	button_grid.resize(filter_names.size());

	for (int i = 0; i < filter_names.size(); i++) {
		float button_x = 0, button_y = 0;
		if (i == 0) {
			button_x = 880.0f;
			button_y = 100.0f;
		}
		else if ((i + 1) % 2 == 0) { // The second column of the grid.
			if (i - 1 >= 0) {
				button_x = button_grid[i - 1].getPosition().x + GRID_HORIZONTAL_GAP + BUTTON_WIDTH;
				button_y = button_grid[i - 1].getPosition().y;
			}
		}
		else {
			if (i - 2 >= 0) {
				button_x = button_grid[i - 2].getPosition().x;
				button_y = button_grid[i - 2].getPosition().y + GRID_VERTICAL_GAP + BUTTON_HEIGHT;
			}
		}
		button_grid[i].changeTextSettings(arial_font, filter_names[i]);
		button_grid[i].initializeButton(BUTTON_WIDTH, BUTTON_HEIGHT, button_x, button_y, BUTTON_COLOR);
	}
}

void My_App::initializeButtons() {

	open_button.changeTextSettings(arial_font, "Open image");
	save_button.changeTextSettings(arial_font, "Save image");
	save_as_button.changeTextSettings(arial_font, "Save as copy");
	reset_button.changeTextSettings(arial_font, "Reset image");

	open_button.initializeButton(130.0f, 40.0f, 20.0f, 10.0f, BUTTON_COLOR);
	save_button.initializeButton(130.0f, 40.0f, 170.0f, 10.0f, BUTTON_COLOR);
	save_as_button.initializeButton(130.0f, 40.0f, 320.0f, 10.0f, BUTTON_COLOR);
	reset_button.initializeButton(130.0f, 40.0f, 470.0f, 10.0f, BUTTON_COLOR);


	undo.changeTextSettings(arial_font, "Undo");
	undo.initializeButton(100.0f, 35.0f, 700.0f, 10.0f, BUTTON_COLOR);
	
	redo.changeTextSettings(arial_font, "Redo");
	redo.initializeButton(100.0f, 35.0f, 810.0f, 10.0f, BUTTON_COLOR);
	
	merge_horizontal.changeTextSettings(arial_font, "Horizontally");
	merge_horizontal.initializeButton(160.0f, 40.0f, 860.0f, 660.0f, BUTTON_COLOR);
	merge_horizontal.setVisible(false);

	merge_vertical.changeTextSettings(arial_font, "Vertically");
	merge_vertical.initializeButton(160.0f, 40.0f, 1040.0f, 660.0f, BUTTON_COLOR);
	merge_vertical.setVisible(false);

	flip_H.changeTextSettings(arial_font, "Flip Horizontally");
	flip_H.initializeButton(160.0f, 40.0f, 860.0f, 660.0f, BUTTON_COLOR);
	flip_H.setVisible(false);

	flip_V.changeTextSettings(arial_font, "Flip Vertically");
	flip_V.initializeButton(160.0f, 40.0f, 1040.0f, 660.0f, BUTTON_COLOR);
	flip_V.setVisible(false);

	rotate90.changeTextSettings(arial_font, "90");
	rotate90.initializeButton(DEGREE_BUTTON_WIDTH, DEGREE_BUTTON_HEIGHT, 890.0f, 660.0f, BUTTON_COLOR);
	rotate90.setVisible(false);

	rotate180.changeTextSettings(arial_font, "180");
	rotate180.initializeButton(DEGREE_BUTTON_WIDTH, DEGREE_BUTTON_HEIGHT, 980.0f, 660.0f, BUTTON_COLOR);
	rotate180.setVisible(false);

	rotate270.changeTextSettings(arial_font, "270");
	rotate270.initializeButton(DEGREE_BUTTON_WIDTH, DEGREE_BUTTON_HEIGHT, 1070.0f, 660.0f, BUTTON_COLOR);
	rotate270.setVisible(false);

	brightness_up.changeTextSettings(arial_font, "Brightness up");
	brightness_up.initializeButton(160.0f, 40.0f, 860.0f, 660.0f, BUTTON_COLOR);
	brightness_up.setVisible(false);

	brightness_down.changeTextSettings(arial_font, "Brightness down");
	brightness_down.initializeButton(160.0f, 40.0f, 1040.0f, 660.0f, BUTTON_COLOR);
	brightness_down.setVisible(false);

	ok.changeTextSettings(arial_font, "Ok");
	ok.initializeButton(120.0f, 40.0f, 900.0f, 660.0f, BUTTON_COLOR);
	ok.setVisible(false);
	
	cancel.changeTextSettings(arial_font, "Cancel");
	cancel.initializeButton(120.0f, 40.0f, 1030.0f, 660.0f, BUTTON_COLOR);
	cancel.setVisible(false);
}

void My_App::initializeNumberBoxes() {
	// set size
	width_box.setSize(NUMBER_BOX_WIDTH, NUMBER_BOX_HEIGHT);
	height_box.setSize(NUMBER_BOX_WIDTH, NUMBER_BOX_HEIGHT);

	// set font
	width_box.setFont(arial_font);
	height_box.setFont(arial_font);

	// set label text
	width_box.setLabelText("Width");
	height_box.setLabelText("Height");

	// set position
	width_box.setPosition(850, 650);
	height_box.setPosition(980, 650);
}

void My_App::adjustToFit() {
	// The image will take space <= 800x600px in the app begin from (20, 100)
	// The center of the image will be at the center of this 800x600 space
	if (list_of_changes.empty()) {
		sf::Vector2f image_size = original_image.getSize();
		
		if (image_size.x > 800 && image_size.y > 600) {
			original_image.setSize(800.0f, 600.0f);
		}
		else if (image_size.x > 800) {
			original_image.setSize(800.0f, image_size.y);
		}
		else if (image_size.y > 600) {
			original_image.setSize(image_size.x, 600.0f);
		}

		float image_x = IMAGE_CENTER_X - image_size.x / 2;
		float image_y = IMAGE_CENTER_Y - image_size.y / 2;
		original_image.setPosition(image_x, image_y);
	}
	else {
		sf::Vector2f image_size = list_of_changes.top().getSize();

		if (image_size.x > 800 && image_size.y > 600) {
			list_of_changes.top().setSize(800.0f, 600.0f);
		}
		else if (image_size.x > 800) {
			list_of_changes.top().setSize(800.0f, image_size.y);
		}
		else if (image_size.y > 600) {
			list_of_changes.top().setSize(image_size.x, 600.0f);
		}


		float image_x = IMAGE_CENTER_X - image_size.x / 2;
		float image_y = IMAGE_CENTER_Y - image_size.y / 2;
		list_of_changes.top().setPosition(image_x, image_y);
	}
}

void My_App::eventHandling(sf::Event& main_event) {
	if (main_event.type == sf::Event::Closed) {
		// Check if we are editing an image
		if (loseChangesWarning() != CANCEL) {
			window.close();
		}
		return;
	}

	if (open_button.isButtonPressed(window, main_event)) {
		// check if there is an image and we did something to it
		if (loseChangesWarning() == CANCEL) return;

		if (original_image.Image::loadFromFile()) {
			isUploadedImage = true;
			original_image.updateTexture();
		}
		return;
	}

	if (save_button.isButtonPressed(window, main_event) && isUploadedImage) {
		if (!list_of_changes.empty()) {
			// Save the image with the last changes
			if (list_of_changes.top().saveChanges()) {
				original_image = list_of_changes.top();
				// if the saving process was done successfully delete the changes
				while (!list_of_changes.empty()) list_of_changes.pop();
				while (!changes_to_redo.empty()) changes_to_redo.pop();
			}
		}
		return;
	}

	if (save_as_button.isButtonPressed(window, main_event) && isUploadedImage) {
		stu::Texture temp = list_of_changes.empty() ? original_image : list_of_changes.top();
		if (temp.saveCopy()) {
			while (!list_of_changes.empty()) list_of_changes.pop();
			while (!changes_to_redo.empty()) changes_to_redo.pop();
		}
		return;
	}

	if (reset_button.isButtonPressed(window, main_event) && isUploadedImage) {
		// Update the image with the original pixels
		original_image.updateTexture();
		while (!list_of_changes.empty()) list_of_changes.pop();
		while (!changes_to_redo.empty()) changes_to_redo.pop();
		return;
	}

	if (undo.isButtonPressed(window, main_event)) {
		// Delete the last change

		if (!list_of_changes.empty()) {
			
			changes_to_redo.push(
				list_of_changes.top()
			);
			list_of_changes.pop();
		}

		// Get the pixels of the displayed texture
		if (!list_of_changes.empty()) {
			list_of_changes.top().updateTexture();
		}
		else {
			original_image.updateTexture();
		}
		return;
	}

	if (redo.isButtonPressed(window, main_event)) {

		if (!changes_to_redo.empty()) {

			changes_to_redo.top().updateTexture(); // It will be displayed so we should update the texture with its pixels
			list_of_changes.push(
				changes_to_redo.top()
			);
			changes_to_redo.pop();
		}
	}

	if (merge_horizontal.isButtonPressed(window, main_event)) {
		stu::Texture left_image = list_of_changes.empty() ? original_image : list_of_changes.top();
		stu::Image right_image;

		if (!right_image.loadFromFile()) {
			// The user didn't choose an image
			return;
		}

		left_image.merge_horizontally(right_image);

		left_image.updateTexture();
		list_of_changes.push(left_image);
		merge_horizontal.setVisible(false);
		merge_vertical.setVisible(false);
		isFilterDone = true;
		return;
	}

	if (merge_vertical.isButtonPressed(window, main_event)) {
		stu::Texture left_image = list_of_changes.empty() ? original_image : list_of_changes.top();
		stu::Image right_image;

		if (!right_image.loadFromFile()) {
			// The user didn't choose an image
			return;
		}

		left_image.merge_vertically(right_image);

		left_image.updateTexture();
		list_of_changes.push(left_image);
		merge_horizontal.setVisible(false);
		merge_vertical.setVisible(false);
		isFilterDone = true;
		return;
	}

	if (isResizingEvent(main_event)) {
		return;
	}

	if (flip_V.isButtonPressed(window, main_event) || flip_H.isButtonPressed(window, main_event)) {
		stu::Texture temp = list_of_changes.empty() ? original_image : list_of_changes.top();
		if (flip_H.isButtonPressed(window, main_event)) {
			temp.flip_horizontally();
		}
		else {
			temp.flip_vertically();
		}
		temp.updateTexture();
		list_of_changes.push(temp);
		flip_H.setVisible(false);
		flip_V.setVisible(false);
		isFilterDone = true;
		return;
	}

	if (rotate90.isButtonPressed(window, main_event) || rotate180.isButtonPressed(window, main_event) || rotate270.isButtonPressed(window, main_event)) {
		stu::Texture temp = list_of_changes.empty() ? original_image : list_of_changes.top();
		if (rotate90.isButtonPressed(window, main_event)) {
			temp.rotate_image_90();
		}
		else if (rotate180.isButtonPressed(window, main_event)) {
			temp.rotate_image_180();
		}
		else {
			temp.rotate_image_270();
		}
		temp.updateTexture();
		list_of_changes.push(temp);
		rotate90.setVisible(false);
		rotate180.setVisible(false);
		rotate270.setVisible(false);
		isFilterDone = true;
		return;
	}

	if (brightness_up.isButtonPressed(window, main_event) || brightness_down.isButtonPressed(window, main_event)) {
		stu::Texture temp = list_of_changes.empty() ? original_image : list_of_changes.top();
		if (brightness_up.isButtonPressed(window, main_event)) {
			temp.increase_brightness();
		}
		else {
			temp.decrease_brightness();
		}
		temp.updateTexture();
		list_of_changes.push(temp);
		brightness_up.setVisible(false);
		brightness_down.setVisible(false);
		isFilterDone = true;
		return;
	}

	if (isCroppingEvent(window, main_event)) {
		return;
	}

	// Check if one of the buttons in the grid has been pressed
	if (isButtonGridEvent(main_event)) {
		return;
	}
}

bool My_App::isButtonGridEvent(sf::Event& main_event) {
	if (!isFilterDone || !isUploadedImage) return false;

	for (int i = 0; i < button_grid.size(); i++) {

		if (button_grid[i].isButtonPressed(window, main_event)) {

			// apply the filter then open the image with the new data
			stu::Texture temp;
			if (list_of_changes.empty()) {
				temp = original_image;
			}
			else {
				temp = list_of_changes.top();
			}

			switch (i) {
			case stu::Grayscale:

				if (!temp.applied_filters[stu::Grayscale]) {
					//temp = photo_copy;
					temp.apply_grayscale();
					temp.updateTexture();
					list_of_changes.push(temp);
				}
				break;

			case stu::BlackandWhite:

				if (!temp.applied_filters[stu::BlackandWhite]) {
					//temp = photo_copy;
					temp.apply_BlackandWhite();
					temp.updateTexture();
					list_of_changes.push(temp);
				}
				break;

			case stu::Negative:

				if (!temp.applied_filters[stu::Negative]) {
					//temp = photo_copy;
					temp.invert_image();
					temp.updateTexture();
					list_of_changes.push(temp);
				}
				break;

			case stu::EdgeDetection:

				if (!temp.applied_filters[stu::EdgeDetection]) {
					//temp = photo_copy;
					temp.detect_edges();
					temp.updateTexture();
					list_of_changes.push(temp);
				}
				break;

			case stu::Merge:

				merge_horizontal.setVisible(true);
				merge_vertical.setVisible(true);
				isFilterDone = false;
				break;

			case stu::Flip:

				flip_H.setVisible(true);
				flip_V.setVisible(true);
				isFilterDone = false;
				break;

			case stu::Rotate:

				isFilterDone = false;
				rotate90.setVisible(true);
				rotate180.setVisible(true);
				rotate270.setVisible(true);
				break;

			case stu::Brightness:

				isFilterDone = false;
				brightness_up.setVisible(true);
				brightness_down.setVisible(true);
				break;

			case stu::Crop:

				isFilterDone = false;
				if (!list_of_changes.empty()) croppingRec.setTexture(&list_of_changes.top());
				croppingRec.setVisible(true);
				ok.setVisible(true);
				cancel.setVisible(true);
				break;

			case stu::Frame:

				temp.add_frame(10);
				temp.updateTexture();
				list_of_changes.push(temp);
				break;

			case stu::Resize:

				width_box.setVisible(true);
				height_box.setVisible(true);
				isFilterDone = false;
				break;

			case stu::Blur:

				temp.apply_mean_blur();
				temp.updateTexture();
				list_of_changes.push(temp);
				break;

			default:
				break;
			}
			return true;
		}
	}
	return false;
}

bool My_App::isCroppingEvent(sf::RenderWindow& window, sf::Event& main_event) {
	if (croppingRec.isRectanglePressed(window, main_event)) {
		return true;
	}

	if (croppingRec.isRectangleReleased(window, main_event)) {
		return true;
	}

	if (croppingRec.isRectangleMoved(window, main_event)) {
		return true;
	}

	if (croppingRec.isMovedWithKey(window, main_event)) {
		return true;
	}

	if (ok.isButtonPressed(window, main_event)) {
		sf::Vector2f image_begin = croppingRec.getNewImageBegin();
		sf::Vector2f image_size = croppingRec.getNewImageSize();
		
		stu::Texture temp = list_of_changes.empty() ? original_image : list_of_changes.top();
		temp.crop(image_begin.x, image_begin.y, image_size.x, image_size.y);
		temp.updateTexture();
		list_of_changes.push(temp);

		isFilterDone = true;
		croppingRec.setVisible(false);
		ok.setVisible(false);
		cancel.setVisible(false);
		return true;
	}

	if (cancel.isButtonPressed(window, main_event)) {
		isFilterDone = true;
		croppingRec.setVisible(false);
		ok.setVisible(false);
		cancel.setVisible(false);
		return true;
	}

	return false;
}

bool My_App::isResizingEvent(sf::Event& main_event) {
	if (width_box.isBoxPressed(window, main_event) && !height_box.isactive()) {
		width_box.setActive(true);
		return true;
	}

	if (height_box.isBoxPressed(window, main_event) && !width_box.isactive()) {
		height_box.setActive(true);
		return true;
	}

	if (width_box.isTyping(main_event)) {
		std::string temp1 = width_box.getTypedText(main_event);
		if (!temp1.empty()) resizing_width = temp1;
		return true;
	}

	if (height_box.isTyping(main_event)) {
		std::string temp2 = height_box.getTypedText(main_event);
		if (!temp2.empty()) resizing_height = temp2;
		return true;
	}
	return false;
}

int My_App::loseChangesWarning() {
	if (isUploadedImage && !list_of_changes.empty()) {
		// Let the user choose to save the changes or ignore it
		int button_number = tinyfd_messageBox(
			NULL,
			"Do you want to save this image first?",
			"yesnocancel",
			"question",
			1
		);
		// 0 -> cancel -> Don't do anything
		// 1 -> yes -> Save the changes then clear the stack
		// 2 -> no -> Clear the stack of changes
		if (button_number == YES) {
			if (list_of_changes.top().saveChanges()) {
				while (!list_of_changes.empty()) list_of_changes.pop();
			}
			return YES;
		}
		else if (button_number == NO) {
			while (!list_of_changes.empty()) list_of_changes.pop();
			// Reset the image
			original_image.updateTexture();
			return NO;
		}
		else {
			return CANCEL;
		}
	}
	return -1; // There is no changes
}

void My_App::draw(sf::RenderWindow& window) {

	undo.draw(window);
	redo.draw(window);

	adjustToFit();

	if (!list_of_changes.empty()) {
		list_of_changes.top().draw(window);
	}
	else {
		original_image.draw(window);
	}

	for (int i = 0; i < button_grid.size(); i++) {
		button_grid[i].draw(window);
	}

	open_button.draw(window);
	save_button.draw(window);
	save_as_button.draw(window);
	reset_button.draw(window);

	merge_horizontal.draw(window);
	merge_vertical.draw(window);

	width_box.draw(window);
	height_box.draw(window);

	flip_H.draw(window);
	flip_V.draw(window);

	rotate90.draw(window);
	rotate180.draw(window);
	rotate270.draw(window);

	brightness_up.draw(window);
	brightness_down.draw(window);

	croppingRec.draw(window);
	ok.draw(window);
	cancel.draw(window);
}

void My_App::run() {

	while (window.isOpen()) {
		sf::Event main_event;

		while (window.pollEvent(main_event)) {
			
			eventHandling(main_event);

		}

		// Logic

		// Applying the resize filter
		if (!resizing_width.empty() && !resizing_height.empty() && isFilterDone == false) {
			int width_value = stoi(resizing_width);
			int height_value = stoi(resizing_height);

			// Edit the last image
			stu::Texture temp;
			if (list_of_changes.empty()) {
				temp = original_image;
			}
			else {
				temp = list_of_changes.top();
			}

			temp.resize(width_value, height_value);
			temp.updateTexture();
			list_of_changes.push(temp);

			resizing_width.clear(); resizing_height.clear();
			width_box.reset(); height_box.reset();
			isFilterDone = true;
		}


		// Update the window contents
		window.clear(BACKGROUND_COLOR);

		draw(window);

		window.display();
	}
}