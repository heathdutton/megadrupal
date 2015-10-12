Overview
--------
This module provides an API and integration layer for using the
CSS-Crush (or Crush) CSS preprocessor in Drupal.

http://the-echoplex.net/csscrush


Basic installation
------------------
Before using the module you must download the Crush library from
Github into your Drupal installation:

https://github.com/peteboere/css-crush/zipball/master

Un-zip the download. Rename the folder to 'csscrush' and place in your site
libraries folder (create it if it doesn't exist):

sites/all/libraries/csscrush


Composer installation
---------------------
Crush integrates with the Composer Manager module which can manage the
library dependency automatically:

https://drupal.org/project/composer_manager

If you manage your composer dependencies elsewhere the following command will
append the necessary require statement to your composer.json file:

  $ composer require css-crush/css-crush:dev-master

The library will then autoload on demand provided you have linked the
composer generated autoload file.


Usage
-----
This module provides two ways of integrating with the Crush preprocessor:

Firstly, module or theme stylesheets with a '.crush.css' file extension will
automatically be detected and preprocessed.

Using this method with theme stylesheets Crush configuration options (scoped
to that theme) can be set in the theme.info file with the following syntax:

  settings[csscrush][options][minify] = 0
  settings[csscrush][options][enable][] = canvas


Secondly, a functional API (based on drupal_add_css()) is available for
preprocessing files and plain strings of CSS and adding them to the page:

  <?php

  /*
    Adding a Crush processed file to your theme (ideally in template.php).
  */
  csscrush_add_css('theme://your-theme/styles.css');

  /*
    Adding a Crush processed file from a module.
  */
  csscrush_add_css('module://your-module/styles.css');

  /*
    Adding Crush processed inline styles.
  */
  csscrush_add_css('body { background: linear-gradient( to bottom, white, silver ); }', 'inline');

  ?>


Functional API
--------------

  csscrush_add_css($data = NULL, $options = NULL, $csscrush_options = array())

Essentially a wrapper for drupal_add_css() with the notable exception that
$option['every_page'] is set to true by default.

Pseudo streams can also be used to reference CSS files with the syntax
'<layer>://<name>/<path>' (see Usage examples).

See drupal_add_css() docs for details on the $options argument:
http://api.drupal.org/api/drupal/includes!common.inc/function/drupal_add_css/7

See CSS-Crush wiki[2] for details on the $csscrush_options argument:
https://github.com/peteboere/css-crush/wiki/PHP-API
