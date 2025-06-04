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
	Image(std::string name);
	Image(Image &img, std::string newfullname);
	~Image();
	bool get_image(std::string name);
	bool save_image();
	bool save_new_image(std::string newImageName, std::string extension);
	bool can_create_new_file(std::string newImageName, std::string extension);
	std::string get_file_extension(std::string name);
	// Filters
	void apply_grayscale();
	void apply_BlackandWhite();
	void apply_invert();
};

#endif