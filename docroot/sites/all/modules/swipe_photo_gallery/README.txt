
CONTENTS OF THIS FILE
---------------------

 * Summary
 * Installation
 * Dependencies
 * Configuration


SUMMARY
------------

You can create portfolio slide, that can be combined into separate albums with taxonomy autocomplete field.
Visitors can see all photos from all albums on one page. Swipe navigation and responsive behavior is included!
Covers of each albums are displayed as slides and make a horizontal row. Up and down there are photos 
from the current album.
Adding new photo:
- go to Add content >> Slide photo and fill the following fields 
	- Title - enter photo name. 
	- Slide image. Choose image from local comp.
	- Slide position - position of photo in album.
	- Album. Album name.
	- Horizontal position of firsts slides. Weight of albums, 1 - first.
	  Necessary if checked First slide.
	- First slide - check if image will be cover of album.


INSTALLATION
------------

1. Copy the 'swipe_photo_gallery' directory to your \sites\all\modules directory

2. Enable the module at admin/modules

3. Check version of jQuery admin/config/development/jquery_update (1.7 and higher req.)


DEPENDENCIES
------------

- Entity API 
- jQuery Update


CONFIGURATION
-------------
The configuration page is at admin/config/media/swipe_photo_gallery,
where you can configure name of created page by Swipe Photo Gallery module and change path to the page. 
After saving new page name flush all caches.
To provide page link to main menu please add this link on admin/structure/menu/manage/main-menu.
You can add links to such kind of photo slider page in any meny or any part of your Drupal site.

Enjoy the module!

