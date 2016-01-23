
-- SUMMARY --

Restricts IMG tags in filtered user input to this site.

For a full description of the module, visit the project page:
  http://drupal.org/project/filter_html_image_secure
To submit bug reports and feature suggestions, or to track changes:
  http://drupal.org/project/issues/filter_html_image_secure


-- INSTALLATION --

* Install as usual, see http://drupal.org/node/70151 for further information.


-- CONFIGURATION --

* Enable the filter for an input format, which also has HTML filter enabled.

* Add "<img>" to allowed HTML tags of HTML filter.

* Re-arrange the filter order so this filter is executed last.

  In particular:
  - after HTML filter, Line-break filter, URL filter, etc.
  - before HTML corrector.


-- CONTACT --

Current maintainers:
* Daniel F. Kudwien (sun) - http://drupal.org/user/54136

