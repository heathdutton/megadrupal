Store Front Theme
--------------

The Store Front theme is designed to work with the Drupal Commerce Project.
Find out more about Drupal Commerce here: http://www.drupalcommerce.org/

This theme can be installed with Commerce Kickstart, and installation profile
for Drupal Commerce (http://drupal.org/project/commerce_kickstart). Store
Front aims to address all the client-side interactions with your Drupal
Commerce installation and clean up the elements that Commerce provides out
of the box.


INSTALLATION
------------
These instructions are assuming that you are using Git and understand how to
use Git. You can read more about Git here: http://drupal.org/documentation/git

 1. Navigate to your sites/all/themes folder

 2. Checkout the 7.x branch from the sandbox using Git by following
 instructions found here:

    http://drupal.org/project/1370548/git-instructions

 3. Download respond.js from https://github.com/scottjehl/Respond. Rename
 respond.min.js to respond.js and place it in the js/ folder.
 Uncomment line 24 in the storefront.info file to initialize respond.js

 4. Navigate to /admin/appearance and find Store Front Core.

 5. Click "Enable and set to default" and the Store Front theme will be active.

 6. Go to /admin/appearance/settings/storefront to set additional variations
 to the theme.
