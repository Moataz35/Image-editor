#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"
#include "tinyfiledialogs.h"
#include "Image.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#define NO_EXTENSION "No extension found"
using namespace std;



stu::Image::Image() {
	imageFullname = "";
	width = height = channels = size = 0;
	image_path_ptr = NULL;
	pixels = NULL;
	applied_filters.resize(FiltersNum);
	image_formats[0] = "*.png" ;
	image_formats[1] = "*.bmp" ;
	image_formats[2] = "*.tga" ;
	image_formats[3] = "*.jpg" ;
}

stu::Image::Image(int width, int height, int channels) : applied_filters(FiltersNum) {
	imageFullname = "";
	image_path_ptr = NULL;
	this->width    = width;
	this->height   = height;
	this->channels = channels;
	this->size     = width * height * channels;
	this->pixels   = new unsigned char[size];
	for (int i = 0; i < size; i++) {
		this->pixels[i] = 0;
	}
	applied_filters.resize(FiltersNum);
	image_formats[0] = "*.png";
	image_formats[1] = "*.bmp";
	image_formats[2] = "*.tga";
	image_formats[3] = "*.jpg";
}

stu::Image::Image(string name) {
	applied_filters.resize(FiltersNum);
	image_formats[0] = "*.png";
	image_formats[1] = "*.bmp";
	image_formats[2] = "*.tga";
	image_formats[3] = "*.jpg";
	get_image(name);
}

stu::Image::Image(const stu::Image& img) {
	this->imageFullname = img.imageFullname;
	this->image_path_ptr = this->imageFullname.c_str();
	this->width    = img.width;
	this->height   = img.height;
	this->channels = img.channels;
	this->size     = width * height * channels;
	this->pixels = new unsigned char[size];
	this->applied_filters = img.applied_filters;
	for (int i = 0; i < size; i++) {
		this->pixels[i] = img.pixels[i];
	}
	this->image_formats[0] = "*.png";
	this->image_formats[1] = "*.bmp";
	this->image_formats[2] = "*.tga";
	this->image_formats[3] = "*.jpg";
}

stu::Image& stu::Image::operator=(const Image& img) {
	stbi_image_free(this->pixels);
	this->imageFullname = img.imageFullname;
	this->image_path_ptr = this->imageFullname.c_str();
	this->width    = img.width;
	this->height   = img.height;
	this->channels = img.channels;
	this->size     = width * height * channels;
	this->pixels = new unsigned char[size];
	this->applied_filters = img.applied_filters;
	for (int i = 0; i < size; i++) {
		this->pixels[i] = img.pixels[i];
	}
	this->image_formats[0] = "*.png";
	this->image_formats[1] = "*.bmp";
	this->image_formats[2] = "*.tga";
	this->image_formats[3] = "*.jpg";
	return *this;
}

stu::Image::~Image() {
	stbi_image_free(pixels);
	cout << "Image has been destroyed!\n";
}

bool stu::Image::loadFromFile() {
	// Open a file dialog to let the user choose the image he want
	image_path_ptr = tinyfd_openFileDialog(NULL, NULL, n_supported_formats, image_formats, NULL, 0);
	if (image_path_ptr == NULL) {
		std::cout << "Failed to load the iamge.\n";
		return false;
	}
	// If the user choose an image and didn't press cancel get the pixels from the image
	pixels = stbi_load(image_path_ptr, &width, &height, &channels, 0);
	size = width * height * channels;
	imageFullname = image_path_ptr;

	if (pixels == NULL) {
		std::cout << "Failed to get the pixels.\n";
		return false;
	}
	return true;
}

bool stu::Image::get_image(string name) {
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
	image_path_ptr = imageFullname.c_str();

	pixels = stbi_load(image_path_ptr, &width, &height, &channels, 0);
	size = width * height * channels;

	if (pixels != NULL) {
		cout << "Read the file successfuly\n";
		return true;
	}
	cout << "Failed to read the file!\n";
	cout << "Wrong file name\n";
	return false;
}

bool stu::Image::saveChanges() {
	string extension = get_file_extension(imageFullname);
	int success = 0;
	if (extension == "png") {
		success = stbi_write_png(image_path_ptr, width, height, channels, pixels, width * channels);
	}
	else if (extension == "bmp") {
		success = stbi_write_bmp(image_path_ptr, width, height, channels, pixels);
	}
	else if (extension == "tga") {
		success = stbi_write_tga(image_path_ptr, width, height, channels, pixels);
	}
	else if (extension == "jpg") {
		success = stbi_write_jpg(image_path_ptr, width, height, channels, pixels, 100);
	}
	if (success) {
		cout << "Image is edited successfuly\n";
		return true;
	}
	cout << "Failed to save the image\n";
	return false;
}

bool stu::Image::saveCopy() {
	// Get the full path of the new image
	char* save_path = tinyfd_saveFileDialog("Saving a copy", NULL, n_supported_formats, image_formats, NULL);

	// Check if there is no path provided
	if (save_path == NULL) {
		std::cerr << "The image is not saved!\n";
		return false;
	}

	// If there is a path check if it's valid
	if (!isSupportedFormat(save_path)) {
		std::cerr << "This format is not supported!\n";
		return false;
	}

	// Check if we have access to the selected directory
	std::ofstream test;
	test.open(save_path);
	if (!test.is_open()) {
		std::cerr << "We don't have access to this directory!\n";
		return false;
	}
	test.close();

	// Save the copy of the image
	int success = 0;
	string extension = get_file_extension(save_path);
	if (extension == ".png") {
		success = stbi_write_png(save_path, width, height, channels, pixels, width * channels);
	}
	else if (extension == ".bmp") {
		success = stbi_write_bmp(save_path, width, height, channels, pixels);
	}
	else if (extension == ".tga") {
		success = stbi_write_tga(save_path, width, height, channels, pixels);
	}
	else if (extension == ".jpg") {
		success = stbi_write_jpg(save_path, width, height, channels, pixels, 100);
	}

	// Check if we successfully saved the image
	if (!success) {
		std::cerr << "Failed to save the image!\n";
		return false;
	}
	return true;
}


// Try to save the new image with the copy constructor
bool stu::Image::save_new_image(string newImageName, string extension) {
	if (!can_create_new_file(newImageName, extension)) {
		cout << "There is already a file with that name!\n";
		return false;
	}
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

bool stu::Image::can_create_new_file(string newImageName, string extension) {
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

bool stu::Image::isSupportedFormat(std::string image_path) {
	std::string extension = "*" + get_file_extension(image_path);
	for (int i = 0; i < n_supported_formats; i++) {
		if (extension == image_formats[i]) {
			return true;
		}
	}
	// If we reach here there is no extension or it's not a supported format
	return false;
}

string stu::Image::get_file_extension(string name) {
	std::string extension;
	for (int i = 0; i < name.size(); i++) {
		if (name[i] == '.') {
			for (int j = i; j < name.size(); j++) {
				extension += name[j];
			}
			break;
		}
	}
	if (extension.empty()) {
		return NO_EXTENSION;
	}
	return extension;
}

void stu::Image::apply_grayscale() {
	if (channels < 3) {
		// It's already doesn't have RGB colors
		return;
	}

	vector<unsigned char> gray_pixels(width * height);
	// Grayscale formula that is used in Photoshop and GIMP
	for (int i = 0, grey_i = 0; i < size; i += channels, grey_i++) {
		int grey = (int)(pixels[i]) * 0.3 + (int)(pixels[i + 1]) * 0.59 + (int)(pixels[i + 2]) * 0.11;
		gray_pixels[grey_i] = grey;
	}

	this->size = size / channels;
	this->channels = 1;
	stbi_image_free(pixels);
	pixels = new unsigned char[size];

	// Get the new pixels from gray_pixels
	for (int i = 0; i < size; i++) {
		pixels[i] = gray_pixels[i];
	}
	applied_filters[stu::Grayscale] = true;
}

void stu::Image::apply_BlackandWhite() {
	apply_grayscale();
	applied_filters[stu::Grayscale] = false; // It's not a grayscale filter
	// For 2 channels image we want to edit the first pixel only
	for (int i = 0; i < size; i += channels) {
		if ((int)(pixels[i]) > 128) {
			pixels[i] = 255;
		}
		else {
			pixels[i] = 0;
		}
	}
	applied_filters[stu::BlackandWhite] = true;
}

void stu::Image::invert_image() {
	if (channels < 3) {
		return;
	}
	for (int i = 0; i < size; i++) {
		int temp = (int)(pixels[i]);
		pixels[i] = 255 - temp;
	}
	applied_filters[stu::Negative] = true;
}

void stu::Image::rotate_image_90() {
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
	applied_filters[stu::Rotate] = true;
}

void stu::Image::rotate_image_180() {
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
	applied_filters[stu::Rotate] = true;
}

void stu::Image::rotate_image_270() {
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
	applied_filters[stu::Rotate] = true;
}

void stu::Image::resize(int newWidth, int newheight) {
	// Adjust the dimensions according to minimum and maxmimum possible dimensions
	newWidth = min(max(10, newWidth), 1000);
	newheight = min(max(10, newheight), 1000);

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
				if (pixel_idx >= size) pixel_idx = 0;
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
				if (pixel_idx >= size) pixel_idx = 0;
			}
		}
	}
	applied_filters[stu::Resize] = true;
}

void stu::Image::flip_horizontally() {
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
	applied_filters[stu::Flip] = true;
}

void stu::Image::flip_vertically() {
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
	applied_filters[stu::Flip] = true;
}

void stu::Image::crop(int start_x, int start_y, int newWidth, int newheight) {
	newWidth = min(newWidth, width - start_x);
	newheight = min(newheight, height - start_y);

	stu::Image temp(newWidth, newheight, this->channels);
	temp.imageFullname = this->imageFullname;
	temp.image_path_ptr = temp.imageFullname.c_str();

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
		for (int x = start_x; x < newWidth + start_x; x++) {
			for (int ch = 0; ch < channels; ch++) {
				temp.pixels[pixel_idx] = original[y][x][ch];
				pixel_idx++;
			}
		}
	}

	this->operator=(temp);
	//*this = temp;
	applied_filters[stu::Crop] = true;
}

void stu::Image::increase_brightness() {
	for (int i = 0; i < size; i++) {
		if (static_cast<int>(pixels[i]) * 1.5 > 255) {
			pixels[i] = 255;
		}
		else {
			pixels[i] = static_cast<int>(pixels[i]) * 1.5;
			// pixels[i] = pixels[i] + 0.5 * pixels[i]
		}
	}
	applied_filters[stu::Brightness] = true;
}

void stu::Image::decrease_brightness() {
	for (int i = 0; i < size; i++) {
		pixels[i] = static_cast<int>(pixels[i]) * 0.5;
		// pixels[i] = pixels[i] - 0.5 * pixels[i]
	}
	applied_filters[stu::Brightness] = true;
}


void stu::Image::blend(stu::Image& img) {
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

void stu::Image::add_frame(int thickness) {
	// Add a colorful frame to RGB or RGBA images and if the channels is less than 3 it will be a black frame

	// First let the user choose the frame color
	unsigned char RGB[3] = { static_cast<unsigned char>(128) };
	tinyfd_colorChooser("Choose a color", NULL, RGB, RGB);

	width = width + 2 * thickness;
	height = height + 2 * thickness;
	vector<vector<vector<unsigned char>>> edited(height, vector<vector<unsigned char>>(width, vector<unsigned char>(channels)));
	int pixel_idx = 0;
	// First n row, last n column, last n row, last n column all their pixels will be blue
	// n is the thickness of the frame
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			for (int ch = 0; ch < channels; ch++) {
				// If we are in the range of the frame part set the frame color
				if (y < thickness || y >= height - thickness || x < thickness || x >= width - thickness) {

					if (ch < 3) {
						edited[y][x][ch] = RGB[ch];
					}
					else {
						// The alpha channel
						edited[y][x][ch] = 255;
					}
				}
				else { 
					// Set the pixels of the image itself
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

	applied_filters[stu::Frame] = true;
}


void stu::Image::apply_mean_blur() {
	const int N = 7;
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

	applied_filters[stu::Blur] = true;
}

stu::Image stu::merge_horizontally(stu::Image& img1, stu::Image& img2) {
	// Merging 2 images horizontally by resizing the image that is bigger in height
	if (img1.channels != img2.channels) {
		// We don't merge 2 images different in channels
		stu::Image no_image;
		return no_image;
	}

	if (img1.height > img2.height) {
		img1.resize(img1.width, img2.height);
	}
	else if (img1.height < img2.height) {
		img2.resize(img2.width, img1.height);
	}

	stu::Image merged(img1.width + img2.width, img1.height, img1.channels);
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

stu::Image stu::merge_vertically(stu::Image& img1, stu::Image& img2) {
	// Merging 2 images vertically by resizing the image that is bigger in width
	if (img1.channels != img2.channels) {
		// We don't merge 2 images different in channels
		stu::Image no_image;
		return no_image;
	}

	if (img1.width > img2.width) {
		img1.resize(img2.width, img1.height);
	}
	else if (img1.width < img2.width) {
		img2.resize(img1.width, img2.height);
	}

	stu::Image merged(img1.width, img1.height + img2.height, img1.channels);
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

void stu::Image::detect_edges() {
	// Detect edges in an image using Sobel operator
	// First we convert the image to grayscale (we just need the intensity of the pixels)
	apply_grayscale();
	applied_filters[stu::Grayscale] = false; // It's not a grayscale filter

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

	applied_filters[stu::EdgeDetection] = true;
}

void stu::Image::increase_purple() {
	if (channels < 3) return;
	for (int i = 0; i < size; i += channels) {
		int red   = pixels[i];
		int green = pixels[i + 1];
		int blue  = pixels[i + 2];
		red = red * 1.2;
		green = 0.4 * green;
		blue = blue * 1.5;
		pixels[i]	  = min(red, 255);
		pixels[i + 1] = min(green, 255);
		pixels[i + 2] = min(blue, 255);
	}
}

void stu::Image::increase_contrast() {
	float factor = 1.2;
	for (int i = 0; i < size; i++) {
		// Don't change the alpha channel
		if (channels == 2 && (i + 1) % 2 == 0) {
			continue;
		}
		if (channels == 4 && (i + 1) % 4 == 0) {
			continue;
		}
		int pixel_value = pixels[i];
		/*
		Method1:
			Without subtraction if it's a light area increase its intensity otherwise decrease it
			if (pixel_value >= 128) {
				pixel_value *= 1.1f;
			}
			else {
				pixel_value *= 0.9f;
			}
		*/
		/*
		Method2:
			128 is a mid value we consider that a value > 128 is light area otherwise it's dark area
			We subtract 128 to know how far is that value from the mid value
			The absolute value of the result represents how much is this area dark or light
			Multiply it by the factor to increase the light area pixels intensity by 20% and decrease the dark by 20%
			Add 128 again to restore the original details

		*/
		pixel_value = factor * (pixel_value - 128) + 128;
		pixel_value = max(pixel_value, 0);
		pixel_value = min(pixel_value, 255);
		pixels[i] = pixel_value;
	}
}

void stu::Image::increase_yellow() {
	if (channels < 3) return;
	for (int i = 0; i < size; i += channels) {
		int red = pixels[i];
		int green = pixels[i + 1];
		int blue = pixels[i + 2];

		red = red * 1.4f;
		green = green * 1.2f;
		blue = blue * 0.8f;

		pixels[i] = min(red, 255);
		pixels[i + 1] = min(green, 255);
		pixels[i + 2] = max(blue, 0);
	}
}

int keepInRange(int pixel_value) {
	if (pixel_value > 255) {
		pixel_value = 255;
	}
	else if (pixel_value < 0) {
		pixel_value = 0;
	}
	return pixel_value;
}

void stu::Image::make_sunlight_effect() {
	/*
		Make a sunlight effect by first increase the contrast
		Increase contrast -> increase the difference between the dark and light areas by making the light area lighter
		and the dark areas darker
		Second we increase the yellow color to make it like the sunlight
	*/
	if (channels < 3) return;
	float factor = 1.2f;
	for (int i = 0; i < size; i += channels) {
		int red = pixels[i];
		int green = pixels[i + 1];
		int blue = pixels[i + 2];

		red = 1.4f * (factor * (red - 128) + 128);
		green = 1.2f * (factor * (green - 128) + 128);
		blue = 0.8 * (factor * (blue - 128) + 128);

		pixels[i] = keepInRange(red);
		pixels[i + 1] = keepInRange(green);
		pixels[i + 2] = keepInRange(blue);
	}
}

bool stu::Image::isNotTogether() {
	for (int i = 0; i < NOT_TOGETHER; i++) {
		if (applied_filters[i]) {
			return true;
		}
	}
	return false;
}