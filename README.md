# A simple image processor
An application to edit images

In this application the user will be able to do the following on images:

1. Resize
2. Crop
3. Aplly filters
4. Create a new one which is a copy from another

## List of filters

|    **Filters**     |
|--------------------|
| Grary scale filter |
| Black and White    |


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
 and 255). There are no intermediate shades of gray in a true black and white image. Each pixel is
 either black (representing darkness or absence of information) or white (representing brightness
 or presence of information). In digital imaging, black and white images are often created by
 converting color images to grayscale and then applying a threshold to convert shades of gray to
 either black or white.