CONTENTS OF THIS FILE
---------------------
   
 * Introduction
 * Requirements
 * Installation
 * Configuration
 * Maintainers

Introduction
------------

This module works only with the Bootstrap theme in Drupal.
This module can be used to create resposnsive slideshow 
while using Bootstrap theme.

REQUIREMENTS
------------

This module requires the following:
jQuery Update module.
Bootstrap theme.

INSTALLATION
------------

Install as usual,
see https://drupal.org/documentation/install/modules-themes/modules-7
for further information.

Configuration
-------------

After successfully installing the module, you can
create contents for the carousel
using the content type 'Responsive Slideshow'. 

Enable the block 'Responsive Slideshow' to a region.
The carousel will be displayed 
in the front page of the website. 

If the carousel need to be displayed on all pages 
the block 'Responsive Slideshow'
(admin/structure/block/manage/responsive_slideshow
/responsive_slideshow/configure)
can be configured accordingly.

The image style applied on the images in the carousel
are in 1140px 300px dimension.
 
The the image style dimensions of the image can be 
changed by configuring the image style 'responsive_slideshow_style'
(admin/config/media/image-styles/edit/responsive_slideshow_style)
which is created during installation.

MAINTAINERS
-----------

Vineetha Wilson
https://www.drupal.org/u/vineethaw
