Image Node Attach Module that Migrate all image_attach module nodes to new 
image entity reference field in Drupal 7. Any image nodes that participate in 
image_attach will be properly attached with this new image field.

Installation
------------

Copy this module to your module directory and then enable on the admin
modules page. Enable the modules and start work on 
admin/config/media/image-node-attach page.

PERQUISITES
--------------
 - To ensure image_attach module was enabled in D6 site and your site is using 
   that.
 - BACK UP YOUR FILES AND DATABASE.
 - Assuming Migration D6 Content Construction Kit (CCK) to D7 Fields is already
   done. See: https://www.drupal.org/node/1144136
 - Image node data is already converted to Image fields using Field convert 
   module and the image_legacy module.
 - Now visit to admin/config/media/image-node-attach and select the content 
   types where image attach images will be migrated.
  
 USAGE
 ----------
 - Backup your Drupal database.
 - Go to admin/config/media/image-node-attach.
 - Select the content type with newly created field name to which all your 
   images will be migrated.
 - Run this script by clicking on submit button.

Author
------
Pushpinder Rana
https://drupal.org/u/er.pushpinderrana
