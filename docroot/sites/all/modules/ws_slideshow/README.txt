Description
-----------

The module integrate amazing WS Slideshow flash gallery in node system.
Go to http://example.com/ws-slideshow for all nodes (albums).
For field type "File" module add "Display widget" "WS Slideshow" for
presentation gallery.

Module add WS Slideshow content type ready for use - create albums
(nodes) with unlimited images. One node = one album. On example path
(see above) located all existing nodes of type "WS Slideshow node".

Installation
------------
Check and save "File system path".

1. Copy the module directory the Drupal sites/all/modules directory.

2. Get "WS Slideshow" from http://www.ws-slideshow.com/downloads.php.
   Unzip in "/sites/all/libraries". Rename to "ws-slideshow".
   (/sites/all/libraries/ws-slideshow/deploy/ws-slideshow.swf).

3. Login as an administrator. Enable the module in the "Administer" -> "Modules"

4. Use "WS Slideshow node" to create album with unlimited cownt of images.

5. Repeat step 3.

6. See http://example.com/ws-slideshow for all nodes.

Settings:
---------
* Changing size (width, height) of WS Slideshow in node:
   admin/structure/types/manage/node_ws_slideshow/display

* Changing size (width, height) of WS Slideshow in page /ws-slideshow:
   admin/config/content/ws-slideshow

* Changing size (width, height) of WS Slideshow thumbnails:
   in Image style page on style naming "ws_slideshow_thumb" change
   ONLY width and height of image effect "Scale".
   !!! Don't change style name and first effect type.

ToDo:
-----

Support
-------
Please use the issue queue for filing bugs with this module at
http://drupal.org/project/issues/ws_slideshow

Author
------
Svetoslav Stoyanov
http://drupal.org/user/717122