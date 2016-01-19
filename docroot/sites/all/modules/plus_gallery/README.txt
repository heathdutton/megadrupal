
CONTENTS OF THIS FILE
---------------------

* Introduction
* Requirements
* Installation
* Notes


INTRODUCTION
------------

Current Maintainer: Seppe Magiels <me@seppemagiels.com>

Plus Gallery is a module that integrates +Gallery, a responsive photo
gallery. It can be used to display images or albums from Facebook,
Google+, Flickr, Instagram or uploaded to your site.


REQUIREMENTS
------------	
* Libraries (http://drupal.org/project/libraries)
* jQuery Update (http://drupal.org/project/jquery_update)


INSTALLATION
------------
* Install as usual, see http://drupal.org/node/70151 for further information.

* Go to http://plusgallery.net and download the latest version. Then uncompress
  the downloaded file to sites/all/libraries, and rename the folder so you get
  the following path sites/all/libraries/plusgallery.

* Go the the jQuery update settings page (admin/config/development/jquery_update)
  and set the "Default jQuery Version" to 1.7.


NOTES
-----
* The images and albums being used have to be placed public, else it won't
  work! At this time you should only add 1 instance of this field per page.
  So only allow one value in the field settings.

* The integration of Facebook albums is only possible for pages!

* If you want to use Facebook you will need a Access Token, you can find
  instructions here: https://drupal.org/node/1988458#comment-7385046

* If you want to use Flickr you need an API key, you can request one here:
  http://www.flickr.com/services/apps/create/apply

* If you want to use Instagram you need an Access Token, you can get it here:
  http://jelled.com/instagram/access-token

* In the view settings of the content type, make sure the field format is set to "Plus Gallery".


EXAMPLES
--------
* Google+, by Erik Rotteveel, https://www.drupal.org/node/2104297