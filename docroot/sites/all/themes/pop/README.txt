Introduction
============
Pop is a clean, flexible HTML5 base theme built with standard practices from HTML5 Boilerplate, 320 and Up and Less Framework. Pop provides a responsible responsive design with a single adaptive grid to support multiple designs and typography layouts.

Integrated with Less grid overlay script.


Installation
============
1. Download Pop from http://drupal.org/project/pop

2. Unpack the downloaded file, take the entire pop folder and place it in your Drupal installation under one of the following locations:
    sites/all/themes
      making it available to the default Drupal site and to all Drupal sites
      in a multi-site configuration
    sites/default/themes
      making it available to only the default Drupal site
    sites/example.com/themes
      making it available to only the example.com site if there is a
      sites/example.com/settings.php configuration file

  *** NOTE ***: You will need to create the "theme" folder under "sites/all/" or "sites/default/".

  For more information about acceptable theme installation directories, read the sites/default/default.settings.php file in your Drupal installation.

3. Log in as an administrator on your Drupal site and go to Administration > Appearance (admin/appearance). You will see the Pop theme there. You can optionally make Pop the default theme.


Build Your Own Sub-Theme
========================
The base Pop theme is designed to be easily extended by its sub-themes. Avoid modify any of the CSS or PHP files in the pop/ folder; Instead you should create a sub-theme of Pop in a folder outside of the root pop/ folder.

Follow the instructions in pop_starterkit/README_pop_starterkit.txt to start building your own subtheme.


Online Reading
==============
Full documentation on the Pop theme can be found in Drupal's Handbook:
  http://drupal.org/node/xxxxxx   // TODO

Excellent documentation on Drupal theming can be found in the Theme Guide:
  http://drupal.org/theme-guide

HTML5 Boilerplate
  http://html5boilerplate.com/

Less Framework
  http://lessframework.com/

320 and Up
  http://stuffandnonsense.co.uk/projects/320andup/


Credits
=======
- HTML5 based on initial work from Boron (http://drupal.org/project/boron)
- Less grid overlay script based on initial work from RnowM (https://twitter.com/RnowM)