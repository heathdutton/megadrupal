
INSTALLATION
------------

Gulp and Bower are required to manage assets.

First, you will need to install NodeJS.

Install gulp and bower with 'npm install -g gulp bower' from the command line. On some setups, sudo may be required.

From the parent (neato) theme directory, enter 'bower install' in the command line. This will pull in the component assets for Neato. These
are referenced includes from the STARTER theme for you - no need to copy them into the STARTER/subthemes.

Create a subtheme. See the BUILD A THEME WITH DRUSH section below on how to do that.

From your subtheme directory, enter 'npm install' in the command line. This will install the required tools to compile
assets.

Run 'gulp' from the subtheme command line to compile CSS from SASS. Gulpfile.js controls what happens in this process. Feel free to
add your own tools into this file to facilitate development. Saving will trigger a cache rebuild, css/js rebuild, and all BrowserSync browsers to reload.

CORE PATCH
----------

There is a major bug in Drupal 7 that causes a segfault if it encounters an .info file and tries to parse the directory as a theme or module. Some node modules,
like BrowserSync, has a .info file in a subdirectory. This will crash Drupal any time you go to the themes page or clear the cache as it tries to rebuild information.

To get around this, implement the patch found here:

https://www.drupal.org/node/619542#comment-9771891


BUILD A THEME WITH DRUSH
----------------------------------
It is highly encouraged to use Drush to generate a sub theme for editing. Do not edit the parent 'neato' theme!

  1. Enable the Neato theme. It does not need to be set as default.
  2. Enter the drush command: drush ngt [THEMENAME] [Description !Optional]