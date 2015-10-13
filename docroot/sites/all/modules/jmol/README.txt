CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Dependencies
 * Installation
 * Usage
 * Example pages
 * Info for themers

INTRODUCTION
------------

This module adds a display formatter for the file field type to display molecular
structure files in a Jmol interactive java applet. Using the applet, it is
possible to interactively analyse the uploaded molecular structure in 3D.

The formatter settings include options to set:
 * the size of the applet
 * a number of exposed viewing styles (wireframe, sticks, ribbon, ...)
 * the default viewing style
 * a textbox allowing custom Jmol script input
 * the page alignment of the applet
 * the default measurements units (angstroms or nanometers)

The default usage of this module would be to create a new content type with
a file field and to choose "Jmol applet" as formatter in the manage
display section of the content type fields.
For more information on the Jmol script language, visit
http://jmol.sourceforge.net/demo/jssample1/ for some example scripts and visit
http://chemapps.stolaf.edu/jmol/docs/ for a reference of all scripting commands
in Jmol. Also visit http://jmol.sourceforge.net/jslibrary for all the Jmol
applet functionality.

Have fun!

DEPENDENCIES
------------

The Jmol module requires the following modules to be also installed:

 * Libraries

INSTALLATION
------------

1. It is not recommended to place this module in the core modules folder
   (this is the 'module' folder directly under your Drupal installation folder).
   Instead place it (or unzip it) in a module folder where all contributed
   modules should be located e.g. 'sites/all/modules'.
   So you would end up with a folder 'sites/all/modules/jmol' under the main
   Drupal installation folder. This folder now contains the jmol.module file and
   other files belonging to the Jmol module.

2. Download the Jmol library from http://jmol.sourceforge.net/
   This is typically a file named Jmol-13.0.1-binary.zip, depending on
   the version.
   The content of this zip file should be extracted in the sites/all/libraries/jmol
   folder. 
   Ensure that the file 'sites/all/libraries/jmol/Jmol.js' exists.

3. On your Drupal site, go to Administration > Modules and
   activate the Jmol module.

USAGE
-----

1. Go to Administration > Structure > Content type and add a new content type.
   Add at least one field of the field type File to the content type.
   You can also add a file field to an existing content type like
   'Article' or 'Basic page'. Don't forget to allow your favorite molecular
   structure file type to be uploaded. This is by default set to files
   with the extension .txt only.

2. Once you added all your fields click on Manage Displays and for the file
   field choose "Jmol applet" as formatter.

3. Now you can click on the cog wheel besides the field to configure the
   formatter. Don't forget to update and save.

Done! Now any uploaded structure file to this field will be displayed using
the Jmol applet.

EXAMPLE PAGES
-------------

This package actually contains two modules. The above described formatter
and three examples pages with predefined Jmol applets as showcase.
These pages are not created by the formatter, but programatically in the file
examples/jmol_examples.module.
Enabling the 'Jmol example pages' module creates three new pages: 

 1. http://example.com/jmol1
 2. http://example.com/jmol2
 3. http://example.com/jmol3

Each one of them contains a few Jmol applets with some widgets. Refer to the
file examples/jmol_examples.module for the code to create fully customized
Jmol applet pages including CSS styling if you require more features than those
provided by the formatter module.

INFO FOR THEMERS
----------------

The Jmol module implements a theme function to display the Jmol applet with
default theming. This function is called theme_jmol_formatter() and the
variables available for theming are explained in the function's comment.
Theme developers can override the default theme function by e.g. creating
a template file jmol_formatter.tpl.php or jmol-formatter.tpl.php in
sites/all/themes/<theme-name>/. For example, in that template file you can
access the size of the applet by calling $variables['settings']['size'].
Clear your cache to make sure Drupal finds the template file.
