# SkipTo
======

SkipTo is a replacement for your old classic "Skipnav" link, (so please use it
as such)! The SkipTo script creates a drop-down menu consisting of the most
important places on a given web page. The menu will make it easier for keyboard
and screen reader users to quickly jump to the desired location by simply
choosing it from the list of options.

##Installation
Installing SkipTo is just like any other Drupal Module
* Download the tar.gz either through the admin panel, or via cli
  * Extract the contents of the archive
* Download the minified SkipTo.min.js file from the main GitHub page
  * http://paypal.github.io/SkipTo/
  * The minified file is in src/drupal/skipTo/js
* Sensible defaults are preset, but you can customize a few items on the 
admin screen. Customizable items are:
  * Headings to use for the menu
  * Landmark Roles to aggregate
  * Access Key to assign to the menu
  * Whether or not the menu wraps (tab from last item returns focus to the 
  first)

##More information available on the GitHub pages:
http://paypal.github.io/SkipTo/

##Git Clone URI
http://git.drupal.org/sandbox/ronfeathers/2017053.git

##Configuration page
admin/config/content/skipto
