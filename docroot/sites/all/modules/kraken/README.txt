CONTENTS OF THIS FILE
=====================
* INTRODUCTION
* REQUIREMENTS
* INSTALLATION
* CONFIGURATION
* UPGRADING
* TODO

INTRODUCTION
============
This module allows image files to be optimised using the Kraken.io web service
at http://kraken.io. After the initial configuration of a Kraken.io account, an
administrator of your site can then configure image styles with the Kraken
optimize effect.

REQUIREMENTS
============
 * Libraries module https://drupal.org/project/libraries

INSTALLATION
============
 * Install as you would normally install a contributed drupal module. See:
  https://drupal.org/documentation/install/modules-themes/modules-7
  for further information.

 * Install the kraken-php library. The easiest method is to use 'drush kraken'.
  If you don't have access to drush then you need to get the kraken-php library
  from https://github.com/kraken-io/kraken-php. Copying the whole of the
  kraken-php library into /sites/all/libraries is sufficient. The absolute
  requirement is that the file called 'Kraken.php' is available at
  /sites/all/libraries/kraken-php/lib/.

CONFIGURATION
=============

1. Go to /admin/config/media/image-toolkit to enter your Kraken.io api key and
   secret.

2. Create a new style at /admin/config/media/image-styles/add.

3. Choose 'Kraken optimize' in the 'Select new effect' list and add it as an
   effect.

4. Set up an image field via a content type or use an existing image field which
   is intended to be used for krak'd images. Preview images may/may not be set
   to the image style you configured in step 3, above.

5. Choose 'manage dislay' tab in from the content type's configuration pages and
   select and the image style as mentioned in step 3 above.

Steps 4 and 5 are merely guidelines. Themers may choose to do their own thing
using 'theme_image_style', for example. See:

https://api.drupal.org/api/drupal/modules%21image%21image.module/function/theme_image_style/7

Read more about 'Working with images in Drupal 7 and 8, here:

https://drupal.org/documentation/modules/image

UPGRADING
=========
If you are upgrading from 7.x-1.x-alpha1, please run update.php to create the
logging table.

TODO
====
1. Set up tests.
2. Display brief instructions on /admin/config/media/image-toolkit to explain
   where/how to set up an account on kraken.io
3. Provide checkboxes for collecting kraken data in watchdog and the kraken
   table.
4. Log kraken operations against the managed files
5. Provide info on the status and version of the kraken-php library on the
   status page.
