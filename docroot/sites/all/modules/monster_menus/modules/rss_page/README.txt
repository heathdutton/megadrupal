The Drupal 7 version of rss_page can optionally load the SimplePie RSS library
from other locations. The following rules are applied, in order:

1. If the "feeds" module (http://drupal.org/project/feeds) is installed, it is
used to load the library.

2. If the "libraries" module (http://drupal.org/project/libraries) is installed,
and something has defined a path for SimplePie, it is used to load the library.

3. If the file exists in the libraries folder, it is loaded. See below for more
info.

4. Otherwise, the built-in version of SimplePie is loaded.


Installing your own version of SimplePie:

If you do not use the "feeds" module, and none of your other modules use the
"libraries" module to define a path for simplepie, you can install any version
you like, by:

1. Create a "simplepie" folder in sites/all/libraries.

2. Download either the minified or single-file version of SimplePie from
     http://simplepie.org/downloads/
   and copy the result to the simplepie folder you created in Step 1.

  a. If you chose the minifield version, rename it to simplepie.mini.php .

  b. If you chose the single-file version, rename it to simplepie.inc .

NOTE: If you use PHP 5.2, you will likely run into this bug:

  https://github.com/simplepie/simplepie/issues/223

which is still present, as of SimplePie 1.3.1. The version I have included with
this module contains a patch.