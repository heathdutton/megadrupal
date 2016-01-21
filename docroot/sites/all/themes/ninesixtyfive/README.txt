CONTENTS OF THIS FILE
---------------------
 * Introduction
 * Features
 * Installation
 * Useful Information
 * Authors
 * Sponsors


INTRODUCTION
------------

ninesixtyfive is based on many goodies combined together by Zentropy (http://drupal.org/project/zentropy) plus a few more, 
it is a base theme which attempts to bring the best of a couple worlds together:

 * It converts the core template files to HTML5 markup (based on "Boron" * all credit to Scott Vandehey (spaceninja) of Metal Toad Media)
 * It adds useful features from the classic Drupal 6 "Basic" theme
 * It includes the 960 Grid system (based on 960gs http://drupal.org/project/ninesixty).
 * It includes a fluid grid system (based on Omega http://drupal.org/project/omega)
 * It includes an HTMl5-friendly CSS Reset and other tweaks & best practices from Paul Irish's "HTML5 Boilerplate" (http://html5boilerplate.com/)
 * It also features a bunch of other goodies thrown in: more regions, Google Analytics integration, more htaccess tweaks, support for Modernizr (http://www.modernizr.com/) and all sorts of awesomeness
 * It *doesn't* give you a pile of CSS rule you will have to override.

FEATURES
--------

 * 960-pixel-width grid system and fluid grid system with 12/16 columns and RTL support
 * Performance and compatibility improvements from HTML5 Boilerplate
 * Many extra body and node classes for easy theming
 * Extra template suggestion for different site "sections"
 * CSS3 Feature detection from Modernizr
 * Full HTML5 CSS Reset
 * Simple Google Analytics integration via theme settings
 * Bartik compatibility (most of Bartik's goodness is maintained)
 * HTML5 doctype and meta content-type
 * Header and Footer sections marked up with header and footer elements
 * Navigation marked up with nav elements
 * Sidebars marked up with aside elements
 * Nodes marked up with article elements containing header and footer elements
 * Comments marked up as articles nested within their parent node article
 * Blocks marked up with section elements
 * HTML5 shim script suggested for full IE compatibility
 * Search form uses the new input type="search" attribute
 * WAI-ARIA accessibility roles added to primary elements
 * Updates all core modules to HTML5 markup
 * Custom maintenance page


INSTALLATION
------------

 * Bad way - Extract the theme in your sites/all/themes/ directory, enable it and start hacking

 * Good way - 

      * Extract the theme in your sites/all/themes/ directory
      * Create a sub-theme (http://drupal.org/node/225125)
      * In you're sub-theme's .info file put 

            base theme = ninesixtyfive

      * Enable your sub-theme and start hacking


USEFUL INFORMATION
------------------

 * ninesixtyfive applies classes to the <html> tag to enable easier styling for Internet Exporer

   - html.ie6 #selector { ... }
   - htm.ie7.ie6 #selector { ... }

 * Out of the box ninesixtyfive will give you a 960-pixel-fixed-width-16-columns grid system, you can enable the fluid grid by uncommenting fluid-16.css from ninesixtyfive.info file

 * You can also have a 12 columns grid but you will have to modify the grid classes in page.tpl.php


AUTHORS
-------

* Alex Weber (alexweber) - http://drupal.org/user/850856 & http://www.alexweber.com.br
* Leandro Nunes (lnunesbr) - http://drupal.org/user/324393 & http://www.nunesweb.com
* Miguel Trindade (migueltrindade) - http://drupal.org/user/690418/ & http://www.migueltrindade.com.br
* Tsachi Shlidor (tsi) - http://drupal.org/user/322980 & http://rtl-themes.co.il


SPONSORS
--------

This project is made possible by :

* Webdrop (http://webdrop.net.br) 
* Linnovate (http://linnovate.net)
* Many others, credit is left where credit is due.
