# jQZoom

The jQZoom module is a wrapper for the jQuery plugin
[jQZoom](http://www.mind-projects.it/projects/jqzoom/).

From the jQZoom homepage: "jQZoom allows you to realize a small magnifier
window close to the image or images on your web page easily. I decided to build
this jQuery plugin to embed detailed big images in my B2B. So now in few steps
you can have your jQZoom in your website,eCommerce or whatever you want."


## Installation

1. Download the jQZoom module and follow the instruction for [installing
contributed modules](http://drupal.org/node/895232).

2. The jQZoom module utilizes the [Libraries
module](http://drupal.org/project/libraries). If not already installed,
download, enable, and configure the latest versions of the Libraries module.

3. Download the [jQZoom
plugin](http://web.archive.org/web/20140903060206/http://www.mind-projects.it/projects/jqzoom/archives/jqzoom_ev-2.3.zip)
and unzip the archive into your site's 'libraries' directory (usually
/site/all/libraries). Rename the directory to "jqzoom".  The resulting path
should be sites/{all}/libraries/jqzoom and you should be albe to find the
javascript file in "sites/{all}/libraries/jqzoom/js/jquery.jqzoom-core-pack.js"

4. On the Modules > List page, enable the jQZoom module.

5. Visit the Configuration > Media > Image toolkit page to verify that an image
toolkit is installed and working properly.

6. Verify the default jQZoom presets have been configured by visiting the
Configuration > Media > Image Styles page.

7. From the Administration > Structure > Content types area, you can "Manage
Display" of an Image field you wish to configure to use jQZoom. Specify the
presets that you wish to use when displaying the image and the zoom,
respectively.


## Contributions

* The original module is largely based on the Thickbox module. Many thanks to
  all the Thickbox developers and contributors.
* Thanks to surge_martin on drupal.org for updating the module to Drupal 6 and
  for adding an adminitration screen for specifying configuration options.
* Author: Matt V.
* Updated for Drupal 7: wesnick
