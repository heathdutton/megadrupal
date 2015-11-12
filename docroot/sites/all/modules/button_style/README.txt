
-- SUMMARY --

Allows to apply a sexy, graphical button style using CSS Sliding Doors technique
in a generic way.

For a full description of the module, visit the project page:
  http://drupal.org/project/button_style

To submit bug reports and feature suggestions, or to track changes:
  http://drupal.org/project/issues/button_style


-- REQUIREMENTS --

None.


-- INSTALLATION --

* Install as usual, see http://drupal.org/node/70151 for further information.


-- USAGE --

None.


-- CUSTOMIZATION --

* You most likely want to override the background images in your theme's
  style.css.  To do so, place the following images into the sub-directory
  'images/' in your theme:

  - bkg_button.png
  - bkg_button-right.png

  and append the following styles to the file 'style.css':

    .form-button-wrapper, li.button a, a.button {
      background-image: url(images/bkg_button-right.png);
    }
    .form-button-wrapper input, li.button a span, a.button span {
      background-image: url(images/bkg_button.png);
    }

  Further tweaking of 'height' and 'padding' may be necessary.


-- CONTACT --

Current maintainers:
* Daniel F. Kudwien (sun) - http://drupal.org/user/54136

This project has been sponsored by:
* UNLEASHED MIND
  Specialized in consulting and planning of Drupal powered sites, UNLEASHED
  MIND offers installation, development, theming, customization, and hosting
  to get you started. Visit http://www.unleashedmind.com for more information.

