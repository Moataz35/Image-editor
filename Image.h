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
	Image(const Image& img); // The returned value from a function is not a variable of type Image it's a constant
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
	// 4. Merge images
	friend Image merge_horizontally(Image& img1, Image& img2);
	friend Image merge_vertically(Image& img1, Image& img2);
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
	// 10. Detect edges (using sobel operator)
	void detect_edges();
	// 11. Resizing
	void resize(int newWidth, int newheight);
	// 12. Blur
	void apply_mean_blur();

	// Additional
	void blend(Image& img);
	void increase_purple();
	void increase_contrast();
	void increase_yellow();
	void make_sunlight_effect();
};

#endif