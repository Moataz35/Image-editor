#include "Button.h"

Button::Button() {
    shape.setSize({ 100, 50 });
    shape.setFillColor(sf::Color(60, 63, 69));
    shape.setPosition(0, 0);
    isVisible = true;
    pressed = false;
    released = false;
}

Button::Button(sf::Font font, sf::String button_text) {
    main_font = font;

    name.setFont(main_font);
    name.setString(button_text);
    name.setCharacterSize(20);
    name.setFillColor(sf::Color::White);

    shape.setPosition(0, 0);
    shape.setFillColor(sf::Color::Black);

    // Set button size according to text size
    sf::FloatRect text_bounds = name.getLocalBounds();
    shape.setSize({ 40 + text_bounds.width, 40 + text_bounds.height });
    // Set the text at the center of the rectangle button
    centerText();

    isVisible = true;
    pressed = false;
    released = false;
}

void Button::styleSheet(
    float width,
    float height,
    float x,
    float y,
    sf::String font_file_name,
    sf::String text,
    int char_size,
    sf::Color background_color,
    sf::Color text_color,
    bool state
) 
{
    setFont(font_file_name);
    setText(text);
    name.setCharacterSize(char_size);
    setSize(width, height);
    setPosition(x, y);
    setBackgroundColor(background_color);
    setTextColor(text_color);
    setVisible(state);
}

void Button::initializeButton(float width, float height,  float x, float y, sf::Color background_color) {
    setSize(width, height);
    setPosition(x, y);
    setBackgroundColor(background_color);
}

void Button::changeTextSettings(sf::Font font, sf::String text, sf::Color text_color) {
    setFont(font);
    setText(text);
    setTextColor(text_color);
}

bool Button::isButtonPressed(sf::RenderWindow& window, sf::Event& event) {
    // If the mouse left button is pressed and its position was on the button then it has been pressed
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left && isVisible) {
        int mouse_x = sf::Mouse::getPosition(window).x;
        int mouse_y = sf::Mouse::getPosition(window).y;
        // Global Bounds -> according to the window
        // Local Bounds -> according to the shape itself
        if (shape.getGlobalBounds().contains(mouse_x, mouse_y)) {
            pressed = true;
            return true;
        }
    }
    return false;
}

bool Button::isButtonReleased(sf::RenderWindow& window, sf::Event& event) {
    // If the mouse left button is pressed and its position was on the button then it has been pressed
    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        int mouse_x = sf::Mouse::getPosition(window).x;
        int mouse_y = sf::Mouse::getPosition(window).y;
        if (shape.getGlobalBounds().contains(mouse_x, mouse_y)) {
            released = true;
            return true;
        }
    }
    return false;
}

bool Button::isvisible() {
    return isVisible;
}

void Button::setVisible(bool state = false) {
    isVisible = state;
}

void Button::setFont(sf::String font_file) {
    main_font.loadFromFile(font_file);
}

void Button::setFont(sf::Font font_file) {
    main_font = font_file;
}

void Button::setText(sf::String text) {
    name.setFont(main_font);
    name.setString(text);
    name.setCharacterSize(20);
    centerText();
}


void Button::centerText() {
    // Get the bounds of the text
    sf::FloatRect text_bounds = name.getLocalBounds();
    // Get the bounds of the button shape
    sf::Vector2f shape_position = shape.getPosition();
    sf::Vector2f shape_size = shape.getSize();
    // Calculate the center position
    float text_x = shape_position.x + (shape_size.x - text_bounds.width) / 2.0f - text_bounds.left;
    float text_y = shape_position.y + (shape_size.y - text_bounds.height) / 2.0f - text_bounds.top;

    name.setPosition(text_x, text_y);
}

void Button::setPosition(float x, float y) {
    shape.setPosition({ x, y });
    centerText();
}

void Button::setSize(float new_width, float new_height) {
    shape.setSize({ new_width, new_height });
    // If the text width is greater than the available space decrease the character size
    while (name.getLocalBounds().width >= new_width  && name.getCharacterSize() > 1) {
        name.setCharacterSize(name.getCharacterSize() - 1);
    }
    centerText();
}


void Button::setBackgroundColor(sf::Color background) {
    shape.setFillColor(background);
}

void Button::setTextColor(sf::Color text_color) {
    name.setFillColor(text_color);
}

void Button::setFrame(float thickness, sf::Color frame_color) {
    shape.setOutlineThickness(thickness);
    shape.setOutlineColor(frame_color);
}

sf::Vector2f Button::getPosition() {
    return shape.getPosition();
}

sf::Vector2f Button::getSize() {
    return shape.getSize();
}

void Button::applyEffect() {
    float change = 15.0f;

    float shape_width = shape.getSize().x;
    float shape_height = shape.getSize().y;
    shape.setSize({ shape_width - change, shape_height - change });

    float shape_x = shape.getPosition().x;
    float shape_y = shape.getPosition().y;
    shape.setPosition(shape_x + change / 2, shape_y + change / 2);

    name.setCharacterSize(name.getCharacterSize() - 2);
    centerText();
}

void Button::removeEffect() {
    float change = 15.0f;

    float shape_width = shape.getSize().x;
    float shape_height = shape.getSize().y;
    shape.setSize({ shape_width + change, shape_height + change });

    float shape_x = shape.getPosition().x;
    float shape_y = shape.getPosition().y;
    shape.setPosition(shape_x - change / 2, shape_y - change / 2);

    name.setCharacterSize(name.getCharacterSize() + 2);
    centerText();
}

void Button::draw(sf::RenderWindow& window) {

    //if (pressed) shape.setFillColor(sf::Color::White);
    //else shape.setFillColor(sf::Color::Black);

    if (isVisible) {
        window.draw(shape);
        window.draw(name);
    }
}