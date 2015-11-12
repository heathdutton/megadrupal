***********
* README: *
***********

DESCRIPTION:
------------
This module implements ability to use MixItUp filtering for Views.
Plugin page: https://mixitup.kunkalabs.com

REQUIRED MODULES:
-----------------
https://www.drupal.org/project/libraries
https://www.drupal.org/project/jquery_update
https://www.drupal.org/project/views

JQUERY COMPATIBILITY:
---------------------
MixItUp plugin requires jQuery v1.7 or greater.

INSTALLATION:
-------------
1. Download and enable all required modules listed above
2. Download MixItUp plugin jquery.mixitup.min.js to your
   libraries directory. So, file should be available under
   /sites/.../libraries/mixitup/jquery.mixitup.min.js.
   For now, used MixItUp v2.1.7.
3. Download and enable MixItUp Views module.

CONFIGURATION:
--------------
1. Go to edit view page
2. Select MixItUp at format section
3. Go to MixItUp format settings and change default animation settings to your
   own settings, if needed. All animation settings available here:
   https://mixitup.kunkalabs.com/docs/#group-animation
4. If you want enable sorting, You should enable "Use sorting" under
   "MixItUp Sorting settings". Don't forget to type labels there.
5. If you want to restrict vocabularies, You can do it under "MixItUp Vocabulary
   settings"
6. Select "Display all items" at Pager section for making all items available.
7. And you can customize all styles for your needs.


Author:
-------
Alexander Ivanchenko
alexsergivan@gmail.com
