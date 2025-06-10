#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "stb_image.h"
#include "stb_image_write.h"
#include "Image.h"
#define NO_EXTENSION "No extension found"
using namespace std;

Image::Image() {
	imageFullname = "";
	width = height = channels = size = 0;
	imageNamePointer = NULL;
	pixels = NULL;
}

Image::Image(string name, int width, int height, int channels) {
	this->imageFullname = name;
	this->imageNamePointer = imageFullname.c_str();
	this->width = width;
	this->height = height;
	this->channels = channels;
	this->size = width * height * channels;
	this->pixels = new unsigned char[size];
	for (int i = 0; i < size; i++) {
		this->pixels[i] = 0;
	}
}

Image::Image(string name) {
	if (get_image(name)) {
		cout << "Read the file successfuly\n";
	}
	else {
		cout << "Failed to read the file!\n";
	}
}

Image::Image(Image& img, string newfullname) {
	this->imageFullname = newfullname;
	this->imageNamePointer = imageFullname.c_str();
	this->width = img.width;
	this->height = img.height;
	this->channels = img.channels;
	this->size = width * height * channels;
	this->pixels = new unsigned char[size];
	for (int i = 0; i < size; i++) {
		this->pixels[i] = img.pixels[i];
	}
}

Image::~Image() {
	stbi_image_free(pixels);
}

bool Image::get_image(string name) {
	// Check if there is no file extension
	string extension = get_file_extension(name);
	if (extension == NO_EXTENSION) {
		cout << NO_EXTENSION << endl;
		return false;
	}

	// Check if the file exists
	//if (can_create_new_file(name, extension)) {
	//	cout << "There is no image with that name in this directory\n";
	//	return false;
	//}

	// The name is correct
	this->imageFullname = name;
	imageNamePointer = imageFullname.c_str();

	pixels = stbi_load(imageNamePointer, &width, &height, &channels, 0);
	size = width * height * channels;

	if (pixels != NULL) {
		return true;
	}
	cout << "Wrong file name\n";
	return false;
}

bool Image::save_image() {
	string extension = get_file_extension(imageFullname);
	int success = 0;
	if (extension == "png") {
		success = stbi_write_png(imageNamePointer, width, height, channels, pixels, width * channels);
	}
	else if (extension == "bmp") {
		success = stbi_write_bmp(imageNamePointer, width, height, channels, pixels);
	}
	else if (extension == "tga") {
		success = stbi_write_tga(imageNamePointer, width, height, channels, pixels);
	}
	else if (extension == "jpg") {
		success = stbi_write_jpg(imageNamePointer, width, height, channels, pixels, 100);
	}
	if (success) {
		cout << "Image is edited successfuly\n";
		return true;
	}
	cout << "Failed to save the image\n";
	return false;
}


// Try to save the new image with the copy constructor
bool Image::save_new_image(string newImageName, string extension) {
	if (!can_create_new_file(newImageName, extension)) {
		cout << "There is already a file with that name!\n";
		return false;
	}
	newImageName += '.';
	newImageName += extension;
	const char* newImagePointer = newImageName.c_str();
	int success = 0;

	if (extension == "png") {
		success = stbi_write_png(newImagePointer, width, height, channels, pixels, width * channels);
	}
	else if (extension == "bmp") {
		success = stbi_write_bmp(newImagePointer, width, height, channels, pixels);
	}
	else if (extension == "tga") {
		success = stbi_write_tga(newImagePointer, width, height, channels, pixels);
	}
	else if (extension == "jpg") {
		success = stbi_write_jpg(newImagePointer, width, height, channels, pixels, 100);
	}

	if (success) {
		cout << "Image copy is saved successfuly\n";
		return true;
	}
	cout << "Failed to save the image\n";
	return false;
}

bool Image::can_create_new_file(string newImageName, string extension) {
	// If the file is opened then there is already a file with that name
	string fullname = newImageName + '.' + extension;
	ifstream test;
	test.open(fullname);
	if (test.is_open()) {
		test.close();
		return false;
	}
	else {
		test.close();
		return true;
	}
}

string Image::get_file_extension(string name) {
	// Get the file extension to know which type we are working on
	bool isExtension = false;
	string extension = "";
	for (int i = 0; i < name.length(); i++) {
		if (isExtension) {
			extension += name[i];
		}
		if (name[i] == '.') {
			// If we reach '.' we are ready to take the extension
			isExtension = true;
		}
	}
	if (isExtension) {
		return extension;
	}
	return NO_EXTENSION;
}

void Image::apply_grayscale() {
	if (channels < 3) {
		// It's already doesn't have RGB colors
		return;
	}
	// Grayscale formula that is used in Photoshop and GIMP
	for (int i = 0; i < size; i += channels) {
		int gray = (int)pixels[i] * 0.3 + (int)pixels[i + 1] * 0.59 + (int)pixels[i + 2] * 0.11;
		pixels[i] = pixels[i + 1] = pixels[i + 2] = gray;
	}
}

void Image::apply_BlackandWhite() {
	apply_grayscale();
	for (int i = 0; i < size; i++) {
		if ((int)pixels[i] > 128) {
			pixels[i] = 255;
		}
		else {
			pixels[i] = 0;
		}
	}
	return;
}

void Image::apply_invert() {
	if (channels < 3) {
		return;
	}
	for (int i = 0; i < size; i++) {
		int temp = (int)(pixels[i]);
		pixels[i] = 255 - temp;
	}
}

void Image::rotate_image_90() {
	// Rotating an image 90° clockwise
	swap(width, height);
	// Set the pixels rotated in a 2D vector
	int pixel_idx = 0;
	vector<vector<unsigned char>> rotated(height, vector<unsigned char>(width * channels));
	for (int col = (width * channels) - channels; col >= 0; col -= channels) {
		for (int row = 0; row < height; row++) {
			// For each channel in every pixel:
			for (int ch = 0; ch < channels; ch++) {
				rotated[row][col + ch] = pixels[pixel_idx];
				pixel_idx++;
			}
		}
	}
	// Replace the original pixels with the new ones
	pixel_idx = 0;
	for (int row = 0; row < height; row++) {
		for (int col = 0; col < (width * channels); col++) {
			pixels[pixel_idx] = rotated[row][col];
			pixel_idx++;
		}
	}
}

void Image::rotate_image_180() {
	int pixel_idx = 0;
	vector<vector<unsigned char>> rotated(height, vector<unsigned char>(width * channels));
	for (int row = height - 1; row >= 0; row--) {
		for (int col = (width * channels) - channels; col >= 0; col -= channels) {
			// For each channel in every pixel:
			for (int ch = 0; ch < channels; ch++) {
				rotated[row][col + ch] = pixels[pixel_idx];
				pixel_idx++;
			}
			
		}
	}
	// Replace the original pixels with the new ones
	pixel_idx = 0;
	for (int row = 0; row < height; row++) {
		for (int col = 0; col < (width * channels); col++) {
			pixels[pixel_idx] = rotated[row][col];
			pixel_idx++;
		}
	}
}

void Image::rotate_image_270() {
	// Rotating an image 90° clockwise
	swap(width, height);
	// Set the pixels rotated in a 2D vector
	int pixel_idx = 0;
	vector<vector<unsigned char>> rotated(height, vector<unsigned char>(width * channels));
	for (int col = 0; col <= (width * channels) - channels; col += channels) {
		for (int row = height - 1; row >= 0; row--) {
			// For each channel in every pixel:
			for (int ch = 0; ch < channels; ch++) {
				rotated[row][col + ch] = pixels[pixel_idx];
				pixel_idx++;
			}
		}
	}
	// Replace the original pixels with the new ones
	pixel_idx = 0;
	for (int row = 0; row < height; row++) {
		for (int col = 0; col < (width * channels); col++) {
			pixels[pixel_idx] = rotated[row][col];
			pixel_idx++;
		}
	}
}

void Image::resize(int newWidth, int newheight) {
	float xfactor = (float)width / newWidth;
	float yfactor = (float)height / newheight;
	// Save the original pixels to free the pointer
	vector<vector<vector<unsigned char>>> original(height, vector<vector<unsigned char>>(width, vector<unsigned char>(channels)));
	int pixel_idx = 0;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			for (int ch = 0; ch < channels; ch++) {
				original[y][x][ch] = pixels[pixel_idx];
				pixel_idx++;
			}
		}
	}
	vector<vector<vector<unsigned char>>> resized(newheight, vector<vector<unsigned char>>(newWidth, vector<unsigned char>(channels)));
	for (int y = 0; y < newheight; y++) {
		for (int x = 0; x < newWidth; x++) {
			int near_x = (x * xfactor);
			int near_y = (y * yfactor);
			if (near_x >= width) near_x = width - 1;
			if (near_y >= height) near_y = height - 1;

			for (int ch = 0; ch < channels; ch++) {
				resized[y][x][ch] = original[near_y][near_x][ch];
			}
		}
	}

	width = newWidth;
	height = newheight;
	size = width * height * channels;
	stbi_image_free(pixels);

	pixel_idx = 0;
	pixels = new unsigned char[size];
	for (int y = 0; y < newheight; y++) {
		for (int x = 0; x < newWidth; x++) {
			for (int ch = 0; ch < channels; ch++) {
				pixels[pixel_idx] = resized[y][x][ch];
				pixel_idx++;
			}
		}
	}
}

void Image::flip_horizontally() {
	vector<vector<vector<unsigned char>>> flipped(height, vector<vector<unsigned char>>(width, vector<unsigned char>(channels)));
	int pixel_idx = 0;
	for (int y = 0; y < height; y++) {
		for (int x = width - 1; x >= 0; x--) {
			for (int ch = 0; ch < channels; ch++) {
				flipped[y][x][ch] = pixels[pixel_idx];
				pixel_idx++;
			}
		}
	}

	pixel_idx = 0;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			for (int ch = 0; ch < channels; ch++) {
				pixels[pixel_idx] = flipped[y][x][ch];
				pixel_idx++;
			}
		}
	}
}

void Image::flip_vertically() {
	vector<vector<vector<unsigned char>>> flipped(height, vector<vector<unsigned char>>(width, vector<unsigned char>(channels)));
	int pixel_idx = 0;
	for (int y = height - 1; y >= 0; y--) {
		for (int x = 0; x < width; x++) {
			for (int ch = 0; ch < channels; ch++) {
				flipped[y][x][ch] = pixels[pixel_idx];
				pixel_idx++;
			}
		}
	}

	pixel_idx = 0;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			for (int ch = 0; ch < channels; ch++) {
				pixels[pixel_idx] = flipped[y][x][ch];
				pixel_idx++;
			}
		}
	}
}