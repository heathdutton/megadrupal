Drupal6.x Colourise Theme 6.x-2.x
-------------------------

 * Original Design : http://www.styleshout.com/
 * Original Author : Erwin Aligam
 * Creative Commons Attribution 2.5 License
 * Drupal Themer : Sungsit Sawaiwan (Gibbo), http://webzer.net/
 * Project page : http://drupal.org/project/colourise


Inspirations
------------

Erwin Aligam's Colourise 1.0 (http://www.styleshout.com/templates/preview/Colourise1-0/index.html) : Beautiful dark design with fixed width layout , one right sidebar , a nice image header and of course, valid XHTML/CSS. ). Otherwise, I change some typographic display to be more legible and more suitable for both English and Thai language.

And the first version of this Drupal theme was inspired by The 960 Grid System : Nathan Smith's Grid Design CSS framework (http://960.gs). But for version 6.x-2.x, I had removed this great framework for supporting various levels of page width. By the way, the 960 Grid System is an effort to streamline web development workflow by providing commonly used dimensions, based on a width of 960 pixels. That is the great thing for the web developer could try.


Features
--------
* Site name
* Site slogan
* Mission statement
* Top search box
* Shortcut icon
* Primary Links
* User pictures in posts and comment
* Top and Bottom content block regions
* 3 Footer block regions
* Accessibility Navigation
* Tableless design
* W3C Valid XHTML 1.0 strict and CSS 2.1
* Cross-browser compatibility (IE6/7, Opera, Safari, and Firefox)


New Features in Colourise-6.x-2.x
---------------------------------

* Built-in theme settings
* Left, Right, Both sidebars and None sidebar support
* 6 level of page width, from 780px to fluid layout. (inside theme settings)
* jQuery PNG Fix for IE6 and below support (inside theme settings)
* Add/Remove your own customized CSS code (inside theme settings)
* Add/Remove Breadcrumb (inside theme settings)
* Bi-directional (BiDi) support (Need feedback/suggestion from RTL users - Arabic, Farsi, Urdu, Hebrew and more)
* Thai translation for theme settings interface (มีอินเตอร์เฟสภาษาไทยใน theme settings ครับ)


Using Colourise Theme Settings
------------------------------

1. Go to "/admin/build/themes/settings/colourise"

2. Choose one from 6 levels of page width you wish, the theme default width is 960px.

3. Choose "IE Transparent PNG Fix" features if you intent to support IE6 and below. This feature will add a jQuery plugin to fix ugly gray background of tranparent PNG image in IE. Read more documentation at http://jquery.andreaseberhard.de/pngFix/.

4. If want to override theme default stylesheet, just add your style into the file "custom.css" before "Add Customized Stylesheet". This file will keep your customized style separate from the default and easy to upgrade. And when Drupal Colourise new version is released, you just put your "custom.css" overwrite the new one.

5. Breadcrumbs and "Back to Top link" : Just check or uncheck the box to toggle them.

6. Don't forget to "Save Configuration"

7. DONE!


Using Maintenance page
----------------------

Colourise-6.x-2.x have included the maintenance page that will override Drupal default style. Before you can use this feature you have to add variables below into your "settings.php" file.

/* **********************************************
                                             
    $conf['maintenance_theme'] = 'colourise';  
                                             
********************************************** */

OR

/* **********************************************

     $conf = array(
       'theme_default'      => 'colourise',
       'maintenance_theme'  => 'colourise',
     );

/********************************************** */


Performance Boost
-----------------

In production site, you should consider to enable "Page Cache" for performance boost because the Colourise have so many CSS files that may increase page load time. To configure "Page Cache" go to "/admin/settings/performance" then set the options you wish there.


Other Notes
-----------

If you love this theme or appreciated thousand lines of code I wrote and my hundred hours in front of the screen monitor, please consider to fill my tip jar at service[at]webzer[dot]net (paypal).


Buddha Bless Drupal...
Gibbozer