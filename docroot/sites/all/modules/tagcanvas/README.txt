TagCanvas for Drupal
====================

TagCanvas is a Javascript class which will draw and animate a HTML5 canvas based tag cloud. It is based on http://www.goat1000.com/tagcanvas.php. This module is dependent on Tagadelic module (http://drupal.org/project/tagadelic).


Installation
------------

- Install Tagadelic and TagCanvas module in your sites/all/modules or sites/default/modules
- Download TagCanvas jQuery plugin into the sites/all/libraries/tagcanvas from http://www.goat1000.com/tagcanvas.php#links
  So you have
    sites/all/libraries/tagcanvas/jquery.tagcanvas.min.js, and/or
    sites/all/libraries/tagcanvas/jquery.tagcanvas.js
- (Optional) For IE < 9, download Explorer Canvas into sites/all/libraries/excanvas_r3 from http://code.google.com/p/explorercanvas/
  So you have
    sites/all/libraries/excanvas_r3/excanvas.compiled.js
- (Optional) If you are using web font such as @font-your-face (http://drupal.org/project/fontyourface), install and enable the Google Webfont Loader (http://drupal.org/project/google_webfont_loader_api), no configuration needed


Configuration
-------------

- Enable Tagadelic module and configure its taxonomy block
- For each block you want to enable TagCanvas, check the 'Display as TagCanvas' checkbox
- A sensible default TagCanvas options has been provided
- Customize the TagCanvas options, you can find a list of all the available options at http://www.goat1000.com/tagcanvas.php#optionList.
- Additional options provided by this module are 'canvasHeight', 'canvasWidth' in px, or "auto" to take its parent height/width.


Similar modules
---------------
- Cumulus (http://drupal.org/project/cumulus)


Credits
-------
- Initially sponsored by HypnoBirthing.com.my and forDrupal.com
- Developed by myfinejob.com