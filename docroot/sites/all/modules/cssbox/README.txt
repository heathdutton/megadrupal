
-- SUMMARY --

The CssBox is very light-weight module that works as other lightbox modules but
it does not use jQuery or JavaScript for that.

It's driven by CSS, the only JavaScript used here is to output overlay contents
by using document.write() for SEO purposes. You can also choose to output HTML
directly to the DOM without JavaScript.

By default module comes with sample CSS, but you can add your own 
customisations. It supports all modern Desktop and Mobile browsers.

Images are resized to fit width or height of the browser, so you always see
best image possible depending on your screen and orientation.

For multiple images they are groupped to gallery by field, so if one field
has many pictures, they will be grupped to one gallery.

For a full description of the module, visit the project page:
  http://drupal.org/project/cssbox

To submit bug reports and feature suggestions, or to track changes:
  http://drupal.org/project/issues/cssbox


-- REQUIREMENTS --

Image file field to work on.


-- INSTALLATION --

* Install as usual, see http://drupal.org/node/895232 for further information.


-- CONFIGURATION --

Go to "Configuration" -> "Media" -> "CssBox" to find all the 
configuration options.

It integrates with Image field and Image styles. 
Just go to Content Types Manage Display options and change Image field format
to CssBox. This works with Views as well and you can select all the options 
after adding Image field to View fields list.


-- CUSTOMIZATION --

Add a custom CssBox style to your theme:
------------------------------------------
The easiest way to start is by copying the default style cssbox/cssbox.css 
to your current theme under css subdirectory. For example:

* sites/all/themes/custom/MYTHEME/css/cssbox.css

This will load your style instead of the default one.


-- CONTACT --

Current maintainers:
* Tomasz Turczynski (Turek) - https://drupal.org/user/412235

This project has been sponsored by:
* NewsPress
  It was written to be used on http://newspress.pl as a default lightbox.
