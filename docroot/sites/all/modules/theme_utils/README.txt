Welcome to Theme Utils module.

Summary
-------
A set of utilities to make theming a bit easier. Current features:
* Block title to class - Adds the block's title to the block's set of HTML
  classes. Useful when theming to quickly identify and target individual blocks.
* Regions to body class - Adds the page's active regions to the body element's
  list of HTML classes.
* Display @media query - Displays the active @media query in the browser window.
* Display viewport dimensions - Displays the viewport's dimensions in the
  browser window.


Requirements
------------
Chaos Tools (ctools)
Libraries API (libraries) *
X Autoload (xautoload) *
PHP 5.3.0 *

* (optional - required only for displaying @media queries)

Installation
------------
1. Install and enable Theme Utils like every other Drupal module.
2. Configure enabled utilities at admin/config/user-interface/theme_utils

(Optional) If you want to display @media queries:
1. Install Libraries API & X Autoload.
2. Create a directory called 'php-css-parser' in the libraries directory. This
   typically resides at 'sites/all/libraries'. So, after creating the directory,
   you should have 'sites/all/libraries/php-css-parser'.
3. Download version 3.0.0 of PHP-CSS-Parser from the GitHub repo.
   https://github.com/sabberworm/PHP-CSS-Parser (Go to the Tags tab to get the
   link to the 3.0.0 zip file).
4. Extract all files from the zip archive into the 'php-css-parser' directory
   that was created on step 2.
5. Verify that 'Parser.php' is in the following directory:
   [libraries_directory]/php-css-parser/lib/Sabberworm/CSS so, for most
   installations, this will be
   sites/all/libraries/php-css-parser/lib/Sabberworm/CSS


Author
------
Rob Decker (rrrob) - http://drupal.org/user/273533
