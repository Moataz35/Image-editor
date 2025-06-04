#include <iostream>
#include "Image.h"

using namespace std;

int main() {
	// This program apply the filters many times on the same image
	// Create array of flags indicates which filter is applied to this photo
	// Note: After we doing this edit the Black and White filter that uses another filter before it
	Image img("Lenna.png");
	img.apply_invert();
	img.save_new_image("test1", "jpg");
	return 0;
}