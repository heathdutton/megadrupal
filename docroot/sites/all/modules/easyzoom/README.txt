CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Requirements
 * Installation
 * Configuration
   * Easy Zoom Image
   * Easy Zoom Thumbnail
   * Easy Zoom Gallery
 * Troubleshooting
 * Maintainers


INTRODUCTION
------------

Easyzoom adds image formatters for integrating an image with Easy Zoom Jquery
Library: http://i-like-robots.github.io/EasyZoom.  The beauty of Easy Zoom
compared to other zoom modules, is that it is open source and it is mobile
friendly.


REQUIREMENTS
------------

 * Image Module
 * Libraries Module
 * Jquery Update - set at 1.10


INSTALLATION
------------

1. Install as usual, see https://www.drupal.org/node/895232 for further
   information.

2. Download the Easy Zoom Library from Github:
   https://github.com/i-like-robots/EasyZoom/zipball/master.

3. Place the Easy Zoom Library in sites/all/libraries and make sure the folder
   is called easyzoom (The module looks for path:
   sites/all/libraries/easyzoom/dist/easyzoom.js).

3. Enable the Easy Zoom module.

4. Set Jquery to 1.10.


CONFIGURATION
-------------

1. EASY ZOOM IMAGE

This formatter is ideal if you only want to add zoom functionality on one image.
When you set your formatter (For example node display or view display settings),
select image and zoom image style and select zoom type. (keep Has Thumbnails
unchecked).

2. EASY ZOOM THUMBNAIL

This formatter is ideal if you have two fields that return images and one of the
fields contains the main image and the other field contains the thumbnail
images. You will need to create a Easy Zoom Image formatter per step 1 but also
check "Has Thumbnails". On the field that you want to have thumbnails, select
the Easy Zoom Thumbnails and select thumbnail, zoom, and image styles.

3. EASY ZOOM GALLERY

This formatter is ideal if you have one field that contains both the main image
and the thumbnail image. The main image will default to the first image.  When
you set your formatter, select thumbnail, image, and zoom image styles. Also
set what kind of zoom type you will use.


TROUBLESHOOTING
---------------

1. The image doesn't zoom in.

Make sure that the zoomed image is larger than the standard image.

2. There is a javascript error that is breaking the page.

Make sure that you set jQuery to 1.10 via jQuery Update (I haven't tested it on
older version but most likely will work with some older versions).

Make sure you downloaded the library and added it to the sites/all/libraries
folder.

In the libraries folder make sure the folder of the Easy Zoom library is
"easyzoom".


MAINTAINERS
-----------

Current maintainers:
* Albert Jankowski (albertski) - https://www.drupal.org/u/albertski
