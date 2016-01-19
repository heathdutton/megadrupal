README for get_node_img
-----------------------

Info
----
Drupal Version: 6.X
Last update: 2009-05-13 2000 +0000
Author: proggaprogga@gmail.com
Project page: http://drupal.org/project/get_node_img


Introduction
------------
The most convenient way to attach images to Drupal nodes is the CCK imagefield
module.  The get_node_img module provides an easy way to fetch these attached
images.  In the minimum, you need to know the node id and cck field label of the
image.  This is mostly a convenience module for Drupal-based Ajax programming.


Installation
------------
Installation is as usual.  Uncompress and drop the module directory inside
'sites/all/modules' or your site specific module directory.  Then enable it
from 'admin/build/modules' and select appropriate permissions.


Permissions
-----------
This module defines two permissions - get_node_img and set_404_img.  The first
permission decides who can fetch images using this module and the second one
determines who can select the 404 image (see below for details).


Usage
-----
Use URLs of the following form to fetch images:

node_resource/NODE_ID/CCK_LABEL/[IMAGE_NUMBER[.EXTENSION]]

Here,
    NODE_ID is an integer.
    CCK_LABEL is a string.  It's the machine readable label specified during
        the creation of the CCK Image field.  So when the full field label is
        "field_star_image", we only need to mention "star_image".
    IMAGE_NUMBER is integer and it's optional.  It defaults to 0, which refers
        to the first image attached via CCK_LABEL field.
    EXTENSION is string.  It can be anything like png, jpeg, foo, bar, etc.
        The sole reason it's here is because Thickbox expects a standard image
        extension (png/jpg/jpeg/gif) for all image URLs.

Now some example URLs:
    node_resource/10/star_image/
    node_resource/10/star_image/0
    node_resource/10/star_image/0.jpg
    node_resource/10/star_image/0.foo
    node_resource/10/star_image/foo (This also refers to the first image!)
    node_resource/10/star_image/1
    node_resource/10/star_image/4.jpg
    node_resource/11/star_image/3.jpg


HTTP 404
--------
What happens when the requested image is not found?  By default an HTTP 404
status response (File not found) is sent.  But it's also possible to send a
standard placeholder image instead of 404.  This image is selected/deselected
from "admin/settings/get_node_img_404_selection".


Testing
-------
You will probably want to use this module while Ajax programming.  In that case,
my advice is this - try to fetch the image(s) first by typing the image URL(s)
in the browser's address bar.  If you get the expected image, assume that this
module is working fine and proceed to fetch the images through Ajax calls.
OTOH, if you cannot grab the image(s) directly from the browser, then make
sure you have got the URL structure right.  If nothing else works, submit an
issue at drupal.org.


License
-------
GPL v2 (http://www.gnu.org/licenses/old-licenses/gpl-2.0.html)


