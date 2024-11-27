# display-image-in-console
----> Output example for this image is in image.txt <----

-- WARNING -- .bmp format must be at 24-bits colors, or it does not work, to convert it is needed to set the colors at 24 bits, there is also something about bmp format and endianity and or width, then it works at best if the image is a square, width = height. In any case something dipending from -> idx = (int) i*width + j;

-- DESCRIPTION --
Having an image in .bmp format, display it in console made with characters.
Compile gcc with: gcc -o img img.c

Insert in the first lines of the program info such: image name present in the same folder, and how many chars are filling a line in your console (or a few less)
