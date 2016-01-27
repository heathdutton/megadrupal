DESCRIPTION
-----------

Image Preloader module can be used to add a "loader icons" for images. 
It can be used for block, views and content types. On configuration 
setting page it will give you a list of all content types, views page, 
blocks and list of loader icons.

Image Preloader module appends class name "image-preloader-icon" at "image" 
parent html tag, so you can also overright the loader icons from your theme 
css file with following code:

body .image-preloader-icon {
  background-color:#image-background-color-code;
  background-image:url("loader-icon-directory/loader-icon-name.gif");
  background-repeat:no-repeat;
  background-position:center center;
}


You can try out live demonstration of it here:
http://www.home-linen.drupalchamp.com/products/5


INSTALLATION
------------

1. Follow the usual module installation procedure (2).
2. Visit the settings page for global settings.

[2] http://drupal.org/documentation/install/modules-themes/modules-7
[3] admin/config/user-interface/image_preloader

MAINTAINERS
------------
developmenticon.com
