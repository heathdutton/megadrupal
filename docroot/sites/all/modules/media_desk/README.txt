Description
-----------
This module is an extension to the Drupals Media module and together with other 
modules like plupload and manualcrop it provides a slick drag-and-drop interface
 to work with many images in an article.

This is useful for large news sites or portals that have many image formats and
need to cut-down the amount of time used in adding, editing and managing images 
in articles.


Dependencies
------------
1. Media
2. Plupload


Recommended modules
-------------------
1. Manual crop


Installation
------------
To install, copy the media_desk directory and all its contents to your modules
directory.

To enable this module, visit Administration -> Modules, and enable Media Desk.


Usage
-----
This module primarily provides two types of image field widgets. The "Media desk 
selector" and the "Media desk image". 

Media desk selector:
This image field widget is a simple element that allows you to upload multiple
images to your article. It displays them horizontally in a neat row thereby
giving you a clean overview of all the images you would want to use in your 
article. From here you can drag and drop these images onto any "media desk 
image" fields. Always keep the cardinality of this widget greater than 1.

Media desk image:
This image field widget is the actual image field itself. You can use these 
fields as a stand-alone without the "Media desk selector" element. If you have
the "Media desk selector" element then you can drag images from the selector and
drop them into these fields.

It is highly recommended to arrange all your "Media desk image" fields one after
another so that they appear neatly in a horizontal row for best overview 
purpose. This module modifies the theme CSS to arrange these fields neatly.

The "Media desk selector" element can be placed above or below all your "Media 
desk image" elements.   


Troubleshooting
---------------

1. There have been issues using jQuery version 1.8 or above with the media 
module. This really is not a problem with the media desk module but rather
something media module needs to take a look at. Version 1.7 and below should
work fine with this module.

2. Make sure the "Media desk selector" widget always has cardinality greater
than 1. This means the number of values users can enter for this field should
be greater than 1.

3. The 7.x-1.0-alpha2 release integrates the media multiselect module into the
media desk module. Therefore the media multiselect module is not required anymore.
If you have it enabled please disable it to not conflict.
