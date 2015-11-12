CONTENTS OF THIS FILE
---------------------
 
 * Introduction
 * Installation
 * Enabling support for IE


INTRODUCTION
------------

Name:          Domicile Responsive
Project type:  Theme
Base Theme:    NineSixty
Drupal:        7
Project Page:  http://drupal.org/project/domicile_responsive
Maintainer:    Emma Jane Hogbin (http://drupal.org/user/1773)

This theme was originally created by Emma Hogbin for the
Responsive Web Design for Drupal workshop
(www.responsivewebdesignguild.com). It is a sub-theme of NineSixty
(http://drupal.org/project/ninesixty) and a retrofit of the theme 
Domicile (http://drupal.org/project/domicile).

The aim of this theme is to show new themers how a few lines of CSS
can adapt an existing theme to be "responsive". The changes
applied in the theme make radical changes to the original design to 
emphasize the changes. They are probably not appropriate for real 
Web sites, but may act as a good starting point.

This theme also includes a Javascript file which re-writes the order
of the content to put the navigation at the bottom of the screen at 
small screen sizes. You may, or may not, want to include similar 
functionality in your responsive retrofits as it may pose accessibility
concerns.

Another interesting variation which adapts the Domicile design to a 
responsive theme is Detamo (http://drupal.org/project/detamo).



INSTALLATION
------------

1.  Download the NineSixty base theme from http://drupal.org/project/ninesixty.
2.  Unpack the base theme place the folder into sites/all/themes on your 
    Drupal installation directory.
3.  Place this theme into the folder sites/all/themes/domicile_responsive.

Your setup should look like this:
    sites/all/themes/ninesixty
    sites/all/themes/domicile_responsive

The theme Domicile Responsive is now ready for use.

Enable the theme Domicile Responsive
1.  Log into your Drupal site as the site administrator.
2.  Navigate to Administration > Appearance.
3.  Scroll to the bottom of the screen and locate Domicile Responsive.
4.  Click "Enable and set as default". 
    (Note: you do not need to enable NineSixty itself.)
5.  Close the overlay.

Domicile Responsive will be applied to your site. 



ENABLING SUPPORT FOR IE
-----------------------

This theme does not include support for older browsers that do not have
CSS3 media query support. You may wish to use a Javascript polyfill, such 
as respond.js, if you want to include support for these older 
*cough* IE *cough* browsers.

For more information, see: http://drupal.org/project/respondjs
