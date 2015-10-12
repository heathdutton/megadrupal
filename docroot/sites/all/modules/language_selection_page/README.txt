
CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Installation
 * Maintainers

INTRODUCTION
------------

This module allows you to present visitors of your website with a landingpage / splash page
where they can select the language of their choice, based on the languages
that have been enabled on your Drupal site.

This is useful in situations where you don't want to enforce a default language to users
(e.g. because of political or corporate sensitivities) if they come to your website
without a language preference available (url, cookie, ...)

The language selection page can be overridden through Drupal's template system.

It is recommended that you move the "Selection Page" language detection method near the bottom.
Preferably you need to have another method of detection activated above it,
such as "URL" or "Cookie" (via the contrib module http://drupal.org/project/language_cookie),
if you don't want to have users arrive on the language selection page too frequently.

INSTALLATION
------------

1. You need to have the Locale module enabled, and more than 1 enabled language on your website.
2. Copy the module into sites/default/modules or another directory that Drupal can find.
3. Enable the module with Drush or through the web interface.
4. Check the status report page to see if everything is good.
5. Go to admin/config/regional/language/configure to configure language detection and selection
   - enable the "Selection Page" detection method
   - it is recommended to position this method near the bottom, just above "Default".

THEMING
-------
Customize the template by copying the .tpl.php file to your current theme directory.
If you selected "Template in theme", you might want to add a page--languageselection.tpl.php file to your theme, that removes any sidebars or regions that could intervene with the language selection task.

MAINTAINERS
------------
Drupal 6 version and initial Drupal 7 port: Pol Dell'Aiera <drupol@about.me>
Drupal 7 version: Sven Decabooter <sven@svendecabooter.be>