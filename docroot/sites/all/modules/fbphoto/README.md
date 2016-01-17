Facebook Photo/Album Text format for Drupal

Demo: We can try the module on [simplytest.me](http://simplytest.me/project/fbphoto/7.x-1.0-beta1).

### Install

* This module has no dependency, you can enable it like any other Drupal modules.
* Go to /admin/config/content/formats
  * Apply "Facebook photo" filter to which text format we would like to support Facebook photo/album.
  * Reorder the filters, make sure FBPhoto filter processed berfore filter "Convert URLs into links".

### Usage

* Then the filter is ready for our text. Try with:
  * Album: `[fb:https://www.facebook.com/media/set/?set=a.380743478891.164254.8427738891&type=3]`
  * Photo: `[fb:https://www.facebook.com/8427738891/photos/a.380743478891.164254.8427738891/380744538891/]`
* We may need update the CSS to make the picture display nice on current Drupal theme.

### Integration with other modules
* Colorbox (https://drupal.org/project/colorbox)
* Lightbox2 (https://drupal.org/project/lightbox2)
* Attach (https://drupal.org/project/attach)

### Credits

Andy Truong started the module using

- Facebook Graph API
- CURL library

To contribute: Please make Pull Request on [Github](https://github.com/andytruong/fbphoto).
