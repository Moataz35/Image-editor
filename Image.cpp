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

Image::Image(int width, int height, int channels) {
	this->width    = width;
	this->height   = height;
	this->channels = channels;
	this->size     = width * height * channels;
	this->pixels   = new unsigned char[size];
	for (int i = 0; i < size; i++) {
		this->pixels[i] = 0;
	}
}

Image::Image(string name) {
	get_image(name);
}

Image::Image(const Image& img) {
	this->width    = img.width;
	this->height   = img.height;
	this->channels = img.channels;
	this->size     = width * height * channels;
	this->pixels = new unsigned char[size];
	for (int i = 0; i < size; i++) {
		this->pixels[i] = img.pixels[i];
	}
}

Image& Image::operator=(Image& img) {
	stbi_image_free(this->pixels);
	this->width    = img.width;
	this->height   = img.height;
	this->channels = img.channels;
	this->size     = width * height * channels;
	this->pixels = new unsigned char[size];
	for (int i = 0; i < size; i++) {
		this->pixels[i] = img.pixels[i];
	}
	return *this;
}

Image::~Image() {
	stbi_image_free(pixels);
	cout << "Image has been destroyed!\n";
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
		cout << "Read the file successfuly\n";
		return true;
	}
	cout << "Failed to read the file!\n";
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

void Image::apply_greyscale() {
	if (channels < 3) {
		// It's already doesn't have RGB colors
		return;
	}

	vector<unsigned char> grey_pixels(width * height);
	// Grayscale formula that is used in Photoshop and GIMP
	for (int i = 0, grey_i = 0; i < size; i += channels, grey_i++) {
		int grey = (int)(pixels[i]) * 0.3 + (int)(pixels[i + 1]) * 0.59 + (int)(pixels[i + 2]) * 0.11;
		grey_pixels[grey_i] = grey;
	}

	this->size = size / channels;
	this->channels = 1;
	stbi_image_free(pixels);
	pixels = new unsigned char[size];

	// Get the new pixels from gray_pixels
	for (int i = 0; i < size; i++) {
		pixels[i] = grey_pixels[i];
	}
}

void Image::apply_BlackandWhite() {
	apply_greyscale();
	// For 2 channels image we want to edit the first pixel only
	for (int i = 0; i < size; i += channels) {
		if ((int)(pixels[i]) > 128) {
			pixels[i] = 255;
		}
		else {
			pixels[i] = 0;
		}
	}
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
	// Rotating an image 90� clockwise
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
	// Rotating an image 90� clockwise
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

void Image::crop(int start_x, int start_y, int newWidth, int newheight) {
	newWidth = min(newWidth, width - start_x);
	newheight = min(newheight, height - start_y);
	Image temp(newWidth, newheight, this->channels);
	vector<vector<vector<unsigned char>>> original(height, vector<vector<unsigned char>>(width, vector<unsigned char>(channels)));
	int pixel_idx = 0;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			for (int ch = 0; ch < channels; ch++) {
				original[y][x][ch] = this->pixels[pixel_idx];
				pixel_idx++;
			}
		}
	}

	pixel_idx = 0;
	for (int y = start_y; y < newheight + start_y; y++) {
		for (int x = start_x; x < newWidth + start_y; x++) {
			for (int ch = 0; ch < channels; ch++) {
				temp.pixels[pixel_idx] = original[y][x][ch];
				pixel_idx++;
			}
		}
	}

	this->operator=(temp);
	//*this = temp;
}

void Image::increase_brightness() {
	for (int i = 0; i < size; i++) {
		if (static_cast<int>(pixels[i]) * 1.5 > 255) {
			pixels[i] = 255;
		}
		else {
			pixels[i] = static_cast<int>(pixels[i]) * 1.5;
			// pixels[i] = pixels[i] + 0.5 * pixels[i]
		}
	}
}

void Image::decrease_brightness() {
	for (int i = 0; i < size; i++) {
		pixels[i] = static_cast<int>(pixels[i]) * 0.5;
		// pixels[i] = pixels[i] - 0.5 * pixels[i]
	}
}


void Image::blend(Image& img) {
	// Blend two images to a new one Which image1 (the caller) will be more clear

	img.resize(400, 400);
	this->resize(400, 400);

	// Problem when the channels are different
	if (img.channels != this->channels) return;

	vector<vector<vector<unsigned char>>> blended(height, vector<vector<unsigned char>>(width, vector<unsigned char>(channels)));
	int pIdx = 0;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			for (int ch = 0; ch < channels; ch++) {
				int blended_pixel = (this->pixels[pIdx] * 0.5) + (img.pixels[pIdx] * 0.5);
				blended[y][x][ch] = min(blended_pixel, 255);
				this->pixels[pIdx] = blended[y][x][ch];
				pIdx++;
			}
		}
	}
}

void Image::add_frame(int thickness) {
	// Add a colorful frame to RGB or RGBA images and if the channels is less than 3 it will be a black frame
	width = width + 2 * thickness;
	height = height + 2 * thickness;
	vector<vector<vector<unsigned char>>> edited(height, vector<vector<unsigned char>>(width, vector<unsigned char>(channels)));
	int pixel_idx = 0;
	// First n row, last n column, last n row, last n column all their pixels will be blue
	// n is the thickness of the frame
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			for (int ch = 0; ch < channels; ch++) {
				if (y < thickness || y >= height - thickness || x < thickness || x >= width - thickness) {
					if (x == thickness / 2 || y == thickness / 2 || x == width - thickness / 2 - 1 || y == height - thickness / 2 - 1) {
						// Add white lines
						edited[y][x][ch] = 255;
					}
					else {
						if (ch > 1) {
							edited[y][x][ch] = 255;
						}
						else {
							edited[y][x][ch] = 0;
						}
					}
				}
				else {
					edited[y][x][ch] = pixels[pixel_idx];
					pixel_idx++;
					if (pixel_idx >= size) pixel_idx = 0;
				}
			}
		}
	}
	this->resize(width, height);
	pixel_idx = 0;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			for (int ch = 0; ch < channels; ch++) {
				pixels[pixel_idx] = edited[y][x][ch];
				pixel_idx++;
				if (pixel_idx >= size) pixel_idx = 0;
			}
		}
	}
}


void Image::apply_mean_blur() {
	const int N = 9;
	// 5x5 kernel with sigma = 1
	/*vector<vector<int>> kernel = {
		{1,  4,  6,  4, 1},
		{4, 16, 24, 16, 4},
		{6, 24, 36, 24, 6},
		{4, 16, 24, 16, 4},
		{1,  4,  6,  4, 1}
	};*/
	vector<vector<int>> kernel(N, vector<int>(N, 1));
	const int radius = kernel.size() / 2;

	// Save the original pixels
	vector<vector<vector<unsigned char>>> original(height, vector<vector<unsigned char>>(width, vector<unsigned char>(channels)));
	int pixel_idx = 0;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			for (int ch = 0; ch < channels; ch++) {
				original[y][x][ch] = pixels[pixel_idx];
				pixel_idx++;
				if (pixel_idx >= size) pixel_idx = 0;
			}
		}
	}

	// Save the pixels after convolution to a new image (we just use a new 3D vector)
	vector<vector<vector<unsigned char>>> edited(height, vector<vector<unsigned char>>(width, vector<unsigned char>(channels)));
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			for (int ch = 0; ch < channels; ch++) {
				// Move the kernel on the image such that the pixel we are on is at the center of the kernel
				int sum = 0;
				for (int i = y - radius; i <= y + radius; i++) {
					for (int j = x - radius; j <= x + radius; j++) {
						// Handle the edge case
						if (i < height && i >= 0 && j < width && j >= 0) {
							int kernel_i = i - (y - radius);
							int kernel_j = j - (x - radius);
							sum += original[i][j][ch] * kernel[kernel_i][kernel_j];
						}
					}
				}
				edited[y][x][ch] = min(255, sum / (N * N));
			}
		}
	}

	pixel_idx = 0;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			for (int ch = 0; ch < channels; ch++) {
				pixels[pixel_idx] = edited[y][x][ch];
				pixel_idx++;
				if (pixel_idx >= size) pixel_idx = 0;
			}
		}
	}
}

Image merge_horizontally(Image& img1, Image& img2) {
	// Merging 2 images horizontally by resizing the image that is bigger in height
	if (img1.channels != img2.channels) {
		// We don't merge 2 images different in channels
		Image no_image;
		return no_image;
	}

	if (img1.height > img2.height) {
		img1.resize(img1.width, img2.height);
	}
	else if (img1.height < img2.height) {
		img2.resize(img2.width, img1.height);
	}

	Image merged(img1.width + img2.width, img1.height, img1.channels);
	vector<vector<vector<unsigned char>>> edited(merged.height, vector<vector<unsigned char>>(merged.width, vector<unsigned char>(merged.channels)));
	int pixel1_idx = 0, pixel2_idx = 0;
	for (int y = 0; y < merged.height; y++) {
		for (int x = 0; x < merged.width; x++) {
			for (int ch = 0; ch < merged.channels; ch++) {
				if (x < img1.width) {
					edited[y][x][ch] = img1.pixels[pixel1_idx];
					pixel1_idx++;
					if (pixel1_idx >= img1.size) pixel1_idx = 0; // Just for safety
				}
				else {
					edited[y][x][ch] = img2.pixels[pixel2_idx];
					pixel2_idx++;
					if (pixel2_idx >= img2.size) pixel2_idx = 0; // Just for safety
				}
			}
		}
	}

	pixel1_idx = 0;
	for (int y = 0; y < merged.height; y++) {
		for (int x = 0; x < merged.width; x++) {
			for (int ch = 0; ch < merged.channels; ch++) {
				merged.pixels[pixel1_idx] = edited[y][x][ch];
				pixel1_idx++;
				if (pixel1_idx >= merged.size) pixel1_idx = 0;
			}
		}
	}

	return merged;
}

Image merge_vertically(Image& img1, Image& img2) {
	// Merging 2 images vertically by resizing the image that is bigger in width
	if (img1.channels != img2.channels) {
		// We don't merge 2 images different in channels
		Image no_image;
		return no_image;
	}

	if (img1.width > img2.width) {
		img1.resize(img2.width, img1.height);
	}
	else if (img1.width < img2.width) {
		img2.resize(img1.width, img2.height);
	}

	Image merged(img1.width, img1.height + img2.height, img1.channels);
	vector<vector<vector<unsigned char>>> edited(merged.height, vector<vector<unsigned char>>(merged.width, vector<unsigned char>(merged.channels)));
	int pixel1_idx = 0, pixel2_idx = 0;
	for (int y = 0; y < merged.height; y++) {
		for (int x = 0; x < merged.width; x++) {
			for (int ch = 0; ch < merged.channels; ch++) {
				if (y < img1.height) {
					edited[y][x][ch] = img1.pixels[pixel1_idx];
					pixel1_idx++;
					if (pixel1_idx >= img1.size) pixel1_idx = 0;
				}
				else {
					edited[y][x][ch] = img2.pixels[pixel2_idx];
					pixel2_idx++;
					if (pixel2_idx >= img2.size) pixel2_idx = 0;
				}
			}
		}
	}


	pixel1_idx = 0;
	for (int y = 0; y < merged.height; y++) {
		for (int x = 0; x < merged.width; x++) {
			for (int ch = 0; ch < merged.channels; ch++) {
				merged.pixels[pixel1_idx] = edited[y][x][ch];
				pixel1_idx++;
				if (pixel1_idx >= merged.size) pixel1_idx = 0;
			}
		}
	}

	return merged;
}

void Image::detect_edges() {
	// Detect edges in an image using Sobel operator
	// First we convert the image to greyscale
	apply_greyscale();
	int pixels_idx = 0;
	vector<vector<unsigned char>> original_pixels(height, vector<unsigned char>(width));
	vector<vector<unsigned char>> edited(height, vector<unsigned char>(width));
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			original_pixels[y][x] = pixels[pixels_idx];
			pixels_idx++;
			if (pixels_idx >= size) pixels_idx = 0;
		}
	}
	// Sobel operator
	const int radius = 1;
	int kernel[3][3] = {
		{-1, 0, 1},
		{-2, 0, 2},
		{-1, 0, 1}
	};
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int g_x = 0, g_y = 0, gradient = 0;
			// Calculate Gx
			for (int i = y - radius; i <= y + radius; i++) {
				for (int j = x - radius; j <= x + radius; j++) {
					if (i >= 0 && i < height && j >= 0 && j < width) {
						int kernel_i = i - (y - radius); 
						int kernel_j = j - (x - radius);
						g_x += (int)(original_pixels[i][j]) * kernel[kernel_i][kernel_j];
					}
				}
			}
			// Calculate Gy
			for (int i = y - radius, kernel_j = 0; i <= y + radius; i++, kernel_j++) {
				for (int j = x - radius, kernel_i = 2; j <= x + radius; j++, kernel_j--) {
					if (i >= 0 && i < height && j >= 0 && j < width) {
						g_x += (int)(original_pixels[i][j]) * kernel[kernel_i][kernel_j];
					}
				}
			}
			gradient = sqrt(pow(g_x, 2) + pow(g_y, 2));
			if (gradient > 100) {
				edited[y][x] = 0;
			}
			else {
				edited[y][x] = 255;
			}
		}
	}

	pixels_idx = 0;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			pixels[pixels_idx] = edited[y][x];
			pixels_idx++;
			if (pixels_idx >= size) pixels_idx = 0;
		}
	}
}