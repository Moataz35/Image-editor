#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include <vector>
#define FiltersNum 12
#define NOT_TOGETHER 4

namespace stu {

	enum Filter {
		Grayscale, BlackandWhite, Negative, EdgeDetection, Merge, 
		Flip, Rotate, Brightness, Crop, Frame, Resize, Blur
	};
	
	class Image {
	protected:
		static constexpr int n_supported_formats = 4;
		std::string imageFullname;
		int width;
		int height;
		int channels;
		int size;
		const char* image_path_ptr;
		const char* image_formats[n_supported_formats];
		unsigned char* pixels;


		bool can_create_new_file(std::string newImageName, std::string extension);

		bool isSupportedFormat(std::string image_path);
	public:
		std::vector<bool> applied_filters;

		Image();

		Image(int width, int height, int channels);

		Image(std::string name);

		Image(const Image& img); // The returned value from a function is not a variable of type Image it's a constant

		~Image();

		Image& operator=(const Image& img);

		bool loadFromFile();

		bool get_image(std::string name);

		bool saveChanges();

		bool saveCopy();

		bool save_new_image(std::string newImageName, std::string extension);

		// A method to check if there is one of the first NOT_TOGETHER filters are applied on the image
		bool isNotTogether();

		std::string get_file_extension(std::string name);

		int index_3D_to_1D(int width, int height, int channels, int y, int x, int z);

		void index_1D_to_3D(int width, int height, int channels, int index, int& y, int& x, int& z);

		void convert_to_RGB();
		
		// Filters
		// 1. Grayscale
		void apply_grayscale();

		// 2. Black and White
		void apply_BlackandWhite();

		// 3. Negative effect (invert images)
		void invert_image();

		// 4. Merge images
		Image merge_horizontally(Image& img2);
		Image merge_vertically(Image& img2);
		void merge_H(Image& second_img);
		void merge_V(Image& second_img);

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
		void add_frame(int thickness = 10);

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

}


#endif