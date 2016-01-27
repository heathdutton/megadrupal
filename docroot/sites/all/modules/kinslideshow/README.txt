
Description
-----------
This module provides a central function for adding KinSlideshow jQuery plugin
elements. 

REQUIREMENTS
------------

KinSlideshow has two dependencies.

Drupal core modules
1) Block

Contributed modules
2) Libraries API

INSTALLATION
------------

To install KinSlideshow:

1) Copy the Libraries API module to sites/all/modules or sites/sitename/modules
  as you require.
2) Make a folder named kinslideshow in sites/all/libraries/ then download 
  the minified version of the KinSlideshow jQuery plugin
  (http://code.google.com/p/kinslideshow-jquery-plugin/downloads/list) and put
  it to kinslideshow folder.
3) Enable KinSlideshow and all of the modules that it requires.

Usage
-----
The KinSlideshow module provides a block to display KinSlideshow(a kind 
of slideshow).

1) Create an image type field on your Drupal site if you have not already. 
  Administration -> Structure -> Types (admin/structure/types).

2) Add a new custom image style at Administration -> Configuration -> Media ->
  Image styles (admin/config/media/image-styles).

3) Navigate to the block administration  Administration -> Structure -> Blocks
  (admin/structure/block).

4) Configure your KinSlideshow block.

5) Save your KinSlideshow and visit a page URL containing the block to see
   how it appears.

Author
-------
James Wang 
