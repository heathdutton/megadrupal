SUMMARY:
--------
Allows videos embedded using the video_embed_field module to be displayed in a 
DOM Window overlay.


REQUIREMENTS:
-------------
* Libraries. http://drupal.org/project/libraries
* DOM Window. http://drupal.org/project/domwindow
* DOM Window jQuery Plugin. http://swip.codylindley.com/DOMWindowDemo.html
* Image Cache Actions 7.x-1.0 http://drupal.org/project/imagecache_actions
* System Stream Wrapper http://drupal.org/project/system_stream_wrapper
* Video Embed.Field http://drupal.org/project/video_embed_field

* System Stream Wrapper is only needed if you want to use ICA 7.x-1.1 or greater

INSTALLATION:
-------------
* Install this module as usual.
* Make sure the DOM Window is installed correctly and the jquery.DOMWindow was
  downloaded correctly
* If you haven't already done so, add a Video Embed field to one of your content
  types.
* When configuring the field if you enable the description field, it will be 
  used as the link description if you choose not to show the thumbnail.
* The Video Embed overlay settings can be tweaked in the Manage Display tab of 
  your content type. For your video embed field, under the format column you
  will be presented the usual options: Video Player and Thumbnail Preview and a
  new option, provided by this module: Overlay.
* When you change the format, different options will be presented to you in the
  column to the right. So, click on overlay and notice that config settings will
  be presented to you.
* Click on the gear icon on the far right of that row to make the necessary 
  display adjustments.

CONFIGURATION:
--------------
* Video Style.
  Description: Same behavior from the video_embed_field module. Controls the 
  embedded player settings.
  Default value: Normal.

* Show Thumbnail. 
  Description: Controls whether the thumbnail from the video service is 
  displayed or not. If not, then a simple Play Button link will be shown instead
  of the thumbnail.
  Default value: Yes

* Image Style.
  Description: If selected, applies one of the available image styles to the
  thumbnail image. Useful to overlay a play button on top of the thumbnail.
  Default value: None (original image)

COOL EXTRAS:
------------
* For extra oomph try the imagecache_actions module that extends the core image
  functionality.

$ drush dl imagecache_actions; drush en imagecache_canvasactions -y;

ROAD MAP:
---------
* Future enhancements include:
  - More customization of the settings via the user interface / admin screens.

MAINTAINERS:
------------
* Rob Montero (rmontero) - http://drupal.org/user/191552