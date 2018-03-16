# MWPHP
This is a PHP extension for ManateeWorks C library. It uses [PHP-CPP](https://github.com/CopernicaMarketingSoftware/PHP-CPP) to wrap basic function exports provided by the library. 
Uses libjpeg to load JPEG images from which to do the recognition from. It's a good idea to use image converter inside the extension, but my attempt to employ ImageMagick (Magick++) there has failed as I could not figure out how to get raw bytes of the image in the same way
as it's currently done. 

# Installation
Has been created and compiled with g++, PHP 7.2 on Ubuntu Server 16.04 x64
* ManateeWorks library in the same directory for linking
* PHP-CPP configured, compiled and installed
* libjpeg

run mk.sh

## TODO
Add a namespace and class wraps.
