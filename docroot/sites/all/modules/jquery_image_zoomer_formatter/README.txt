********************************************************************
                JQUERY IMAGE ZOOMER FORMATTER 
********************************************************************
Original Author: Varun Mishra
Current Maintainers: Varun Mishra
Email: varunmishra2006@gmail.com

********************************************************************
DESCRIPTION:

   Jquery Image Zoomer Formatter module provides an image formatter
   based on Jquery Zoomer plugin for smooth image exploration. You
   can zoom the images in a very smoothly manner. You can also add
   custom css to customize the zoomer wrapper and other css stuffs
   on zoomer layout.


********************************************************************

INSTALLATION:

1. Place the entire jquery_image_zoomer_formatter directory into sites modules 
  directory (eg sites/all/modules).

2. Enable this module by navigating to:

     Administration > Modules

3) This module have dependency on image, jquery_update and libraries module.

4) Download jQuery Zoomer library from either
   http://plugins.jquery.com/zoomer/ or from
   http://formstone.it/components/zoomer/ . Install it in
   sites/all/libraries directory, and rename the directory to jquery_zoomer.
   The library should be available at a path like
   sites/all/libraries/jquery_zoomer/jquery.fs.zoomer.js.

5) This zoomer plugin requires juqery 1.7 or greater. Please install jquery 
   update module.

6) Please read the step by step instructions as an example to use this
   module below:-

a) Install the module.

b) Go to admin/structure page. Click on manage display of any content type which
   have image field(s).

c) Select jQuery Image Zoomer formatter for the image field on which you would 
   like to use jQuery Zoomer formatter.
