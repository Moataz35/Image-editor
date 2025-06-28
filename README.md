# A simple image processor

An application to edit images
We will use stb_image library to make this app.
In this application the user will be able to do the following on images:

1. Resize
2. Crop
3. Aplly filters
4. Create a new one which is a copy from another

## List of filters

|       Filters      |
|--------------------|
| Grary scale filter |
| Black and White    |
| Negative effect    |
|  Merge two images  |
|     Flipping       |
|     Rotating       |
|    Brightness      |
|     Croping        |
|    Add a frame     |
|  Edge detection    |
|     Resizing       |
|     Bluring        |


## Gray scale filter
 A grayscale image is an image where each pixel is represented by a single sample indicating the intensity of light or shade.

 In grayscale images, there is no color information, only varying shades
 of gray from black to white.

 The intensity value of each pixel determines its brightness, with lower
 values representing darker shades and higher values representing lighter shades.

 
###### Formula
Because humans do not perceive all colors equally, the “average method” of grayscale conversion is inaccurate.  Instead of treating red, green, and blue light equally, a good grayscale conversion will weight each color based on how the human eye perceives it.  A common formula in image processors (Photoshop, GIMP) is:

```
Gray = (Red * 0.3 + Green * 0.59 + Blue * 0.11)
```
Read a good article [here](https://tannerhelland.com/2011/10/01/grayscale-image-algorithm-vb6.html).


## Black and White filter
 Black and white images also lack color, but they typically have only two colors: black and white (0
 and 255). 
 
 There are no intermediate shades of gray in a true black and white image. 
 
 Each pixel is either black (representing darkness or absence of information) or white (representing brightness
 or presence of information). 
 
 In digital imaging, black and white images are often created by converting color images to grayscale and then applying a threshold to convert shades of gray to
 either black or white.

###### Formula
 Usualy we do it by first converting the image to grayscale then we choose a fixed value
 and if a color value is greater than this value we turn it to white color (255) otherwise we turn it to black (0).

 We will use the value 128 as a midpoint.

## Negative effect (invert images)
   Inverting colors is a simple yet powerful imageprocessing operation that transforms the colors of
   an image by reversing each pixel's RGB values.
   
   This operation effectively produces a photographic negative effect, where light areas become dark and vice versa.

###### Formula
 We will just invert the color values (set the values to 255 - color value).

## Merging images
 With this filter we will be able to merge two images horizontally or vertically.

###### Fommula
 First we will resize the big image (the one which has the bigger height in case of merging horizontally) to make it equal to the smaller one.
 After that we will use a vector to save the new pixels of the new big image.

## Flipping image
 Flipping an image involves mirroring its content horizontally or vertically.


## Rotating image
 We will let the user rotate an image by 90° or 180° or 270°.

###### Formula
 We will use a 2D vector to rotate the pixels.

## Change brightness
 This filter allow the user to make the image darker or lighter by 50% from the original image.

## Croping image
 This filter allow the user to choose a start point and the dimensions of a part he/she wants to crop from the image.

## Add a frame
 This allows the user to add a simple frame with different colors.

## Edge detection
 This filter detect the edges in an image.
 An edge in the world of pixels is a sudden change in the intensity of the pixels.
 It's a transition between objects or between an object and a background.

###### Formula
 Using Sobel filter.
 
 It works by calculating the gradient of image intensity at each pixel within the image.
 The sobel filter uses two 3 x 3 kernels one for changes in the horizontal direction, and one for changes in the vertical direction.
 The two kernels are convolved with the original image to calculate the approximations of the derivatives.
 Let's define Gx as the gradient in the x direction and Gy as the gradient in the y direction.
 The gradient magnitude is calculated from Gx and Gy:
 ```
 G = sqrt(pow(Gx, 2) + pow(Gy, 2));
 ```
 We use a threshold to determine is this pixel is a part of an edge or not. If G is greater than the threshold then its a part of an edge.

## Resizing image
 Resizing an image involves changing the dimensions of the image while maintaining the same image details.

## Blur image
 Bluring is a technique used to reduce the level of detail or sharpness in an image, resulting in a smoother appearance.

###### Formula1: Mean blur
 Mean blur also known as average blur or box blur is a simple and widely used image processing tecknique for smoothing an image and reducing noise.

 It works by replacing each pixel's value with the average of the pixel values of its surrounding pixels.

 Mean blur works on the princlple that noise often manifests as isolate, high variations in pixel values.

 By averaging a pixel with its neighbors these extreme variations are smoothed out.

 Read a good article [here](https://how.dev/answers/how-to-blur-an-image-using-a-mean-filter).
