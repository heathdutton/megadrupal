ZipCart - download files as a zip

INTRODUCTION

ZipCart lets your site users bundle files for download in a zip (or other bundle format).

For example: you have a library of audio and image files associated with a topic, and 
want to give users the capacity to select several files for download, then download them
in a single archive.


DEMO

Take a look at http://zipcart.demo.giantrobot.co.nz/ to get an idea of how this works.
This site is running ZipCart 6.x-1.x and implements the bare module - no theme or JS 
enhancements.


USAGE

There are two steps to using this module.

* Visit admin/config/media/zipcart and select the appropriate Zip handler.

* Update your theme with download links to ZipCart, using theme('zipcart_download').
  This function simply wraps l(), so the parameters are similar: $html, $path, $options.
  Eg: You have a file at sites/default/files/file1.txt to make available for download:
  
    <?php print theme('zipcart_download', 'Download file', 'sites/default/files/file1.txt'); ?>

* You need to expose the "ZipCart Downloads" block via admin/structure/block
  This block provides the link for users to build the zip with their files and download it.

* Configure permissions for roles to access ZipCart downloads at admin/people/permissions


JAVASCRIPT ENHANCEMENTS

This module includes JavaScript to improve user experience. If JS is enabled, the user 
will see an animation of the clicked element being copied to the ZipCart Downloads block, 
and the download clicks will be interrupted. The block will also be updated with the 
number of files for download. This JS is dependent on certain CSS classes being present,
and you may want to ensure the ZipCart Downloads block is always visible on screen, or 
is fixed on screen if there are files to download.

If JS is not present or enabled, the module should continue to work as per normal, with
a message displayed to the user as each file is added to the cart.


