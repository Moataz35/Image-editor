#ifndef IMAGE_H
#define IMAGE_H

#include <string>

class Image {
	std::string imageFullname;
	int width;
	int height;
	int channels;
	int size;
	const char* imageNamePointer;
	unsigned char* pixels;
public:
	Image();
	Image(int width, int height, int channels);
	Image(std::string name);
	Image(Image& img);
	~Image();
	Image& operator=(Image& img);
	bool get_image(std::string name);
	bool save_image();
	bool save_new_image(std::string newImageName, std::string extension);
	bool can_create_new_file(std::string newImageName, std::string extension);
	std::string get_file_extension(std::string name);
	// Filters
	// 1. Grayscale
	void apply_grayscale();
	// 2. Black and White
	void apply_BlackandWhite();
	// 3. Negative effect (invert images)
	void apply_invert();
	// 5. Flip image
	void flip_horizontally();
	void flip_vertically();
	// 6. Rotate image
	void rotate_image_90();
	void rotate_image_180();
	void rotate_image_270();
	// 7. Brightness
	void increase_brightness();
	void decrease_brightness();
	// 8. Crop image
	void crop(int start_x, int start_y, int width, int height);
	// 9. Adding frame
	void add_frame(int thickness);
	// 11. Resizing
	void resize(int newWidth, int newheight);

	// Additional
	void blend(Image& img);
};

#endif