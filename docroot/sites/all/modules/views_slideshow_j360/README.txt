=======================
 Views Slideshow: j360
=======================

Integrate Spritespin with the Views Slideshow module.

This module will display a view of images using the spritespin
jQuery plugin available from http://spritespin.ginie.eu/

The stable version of Spritespin is old and seems to suffer of some
flickering problems, so, I've used the development version.
The original dev-version can be dowloaded from this link:
https://github.com/giniedp/spritespin/blob/development/src/spritespin.js

The Spritespin plugin is released under cc-by-nc license:
http://creativecommons.org/licenses/by-nc/3.0
so it can't be included in the module.

Given that, I've modified the dev-version to fit Drupal needs, so you have to 
download this library from my personal website:
http://www.mauromascia.com/files/spritespin-j360.tar.gz
(see the Installation section)


== Spritespin ==

Spritespin is a jQuery plugin that enables sprite animation in your website.

It takes an array of images or a stiched sprite sheet and is able to play
these images frame by frame. This results in an animation.
The aim of this plugin is to provide a 360 degree view of some kind of product.
There is no flash needed.
Everything is done with javascript and the jQuery framework.


==============
 Installation
==============

1. Extract the contents of this project into sites/all/modules/

2. Also download and install
   - views
   - views_slideshow
   - libraries
   - context (optional)
   - multiupload_imagefield_widget (optional)

3. Download the Spritespin library from my personal website:
  
  http://www.mauromascia.com/files/spritespin-j360.tar.gz
  
  Untar it and copy the extracted folder (spritespin-j360) under the libraries
  folder. This must be the final structure:
  
  * sites/all/libraries/spritespin-j360/spritespin-j360.js *
  * sites/all/libraries/spritespin-j360/LICENSE *
   
4. Enable this modules


===============
 Configuration
===============

You must have a content type with a field of type "Image" with the possibility
to upload an unlimited number of images if you want to use "Single images".
If, insteed, you plan to use only one sprite image, you can set it at 1.

We'll call the content type "ct_j360" and the field "field_j360", as example.


== View block ==

Create a view of type "Block" and give it a cool name like "block slideshow
j360" and a title. Then, configure it as follow:

1. FORMAT
  a - Select "Slideshow" (as all other views_slideshow modules)
  b - In the slideshow settings select "j360" as slideshow type.
  c - Configure the j360 options in the "J360 OPTIONS" fieldset.
    You have to choice between:
    -- Single images (default)
    -- Inline spritesheet
    -- Grid spritesheet
    -- Panorama
    You have to configure also the sub-options for the selected type.
Note: For more informations see: http://spritespin.ginie.eu

2. FIELDS
  - Select the image field "field_j360"

3. FILTER CRITERIA
  - Select the content type "ct_j360"

4. CONTEXTUAL FILTERS
  - Select "Content: nid"
  - Then, Under "WHEN THE FILTER VALUE IS NOT AVAILABLE" set it to
     "Provide default value"
  - Select "Content ID from URL" in the Type

If you plan to use only "Single Images" option, you have to do other
things in the view.

5. PAGER
  - Use pager: Display all items | All items
  - More link: No
  
6. RELATIONSHIPS
  - Add the relationship with the image field.
      You will have something like:
      Content: field_j360
    
7. SORT CRITERIA
  - Select "File: Name" and leave "Sort ascending"
    
    Note: This is useful for "Single images" mode.
    Warning: File names MUST be named alphabetically!


== Context ==

You may want to show the previous created block. Ok, let me use context.
After installing/enabling "context" module, go to: admin/structure/context
and create a new context with a cool name.

Now add a condition and a reaction:

- Condition is that the node type must be "ct_j360" (or whatever you call it).
- Reaction: add "Blocks" and then under "Views" select "block slideshow j360"
  (or whatever you call it) and add it into "Content" (or where you want to
  show it).


== Bulk image upload ==

There are several modules that can be used to bulk upload images. One that
I've tested positivly is multiupload_imagefield_widget (with
multiupload_filefield_widget) but the choice is up to you.

If you would like to use/try multiupload_imagefield_widget you can be
interested to fix a little bug, that shows something like this:

  Notice: Undefined index: #size in file_managed_file_process() [...]
          file.module

Here: http://drupal.org/node/1548502 there are various solutions but the #24
worked for me (http://drupal.org/node/1548502#comment-6516930)

If insteed you want to use "inline" or "grid" spritesheet or "panorama"
image, there is no need of the bulk image upload, 'cause you have to upload
just one image.


== FAQ ==

Q: I'm using the "Single images" option but I can't upload all files that I've
   selected! Why?
A: It's probably a "php.ini" limitation: check the value for "max_file_uploads"
   rule.

Q: I'm using the "Single images" option but it seems that images are unordered!
   Why?
A: Check for image names and Sort Order in the view, please.

Q: I'm trying to upload a big image but could not be saved, because it exceeds
   X MB, the maximum allowed size for uploads. How can I solve?
A: It's higly probably that you have to increase the value for
   "upload_max_filesize" rule on the "php.ini".
   
