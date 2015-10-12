CONTENTS OF THIS FILE
---------------------

 * Author
 * Description
 * Installation
 * Configuration
 * Requirements
 * Theming

AUTHOR
------
stred (http://drupal.org/user/956840)

DESCRIPTION
-----------
This module provides a views plugin to display content items form 8 different
locations and 4 directions while scrolling. Can be used to display a single 
page or to build a whole website with multiple sections. 
Sounds complicated? check the demo or the picture lines_position.gif supplied 
with the module.

DEMO
----
default options (work out of the box) 
 http://thomas.lemaire.name/views-coscev/basic
different settings and sections
 http://thomas.lemaire.name/views-coscev/advanced

INSTALLATION
------------
Enable the module like any other Drupal Module /admin/build/modules. The module
name is Views COntent SCroll from EVerywhere and can be found on views package.

USAGE
-----
1) Activate the style plugin in the view -> (Display) Format and chose
    "Content scroll (COSCEV)"
2) Settings:
    - Margins: top, right, bottom and left. Check lines_position.gif
    - Title: inline, hidden or fixed with several effects. 
    Can be displayed in a top navigation bar. Check the demo.
    - Overlap: how far the next item wil appear. See plugin item description
    - View item width: usefull for inline items like text
    - Center first item
    - Directions: random or manual set the 8 different possibilities
    - Random options: avoid unaesthetic sequence
3) Multiple sections:
    create as many attachments as you want sections and adjust titles. 
    See advanced demo

REQUIREMENTS
------------
Views Module installed 

THEMING
-------
A custom page.tpl.php is used because you can only display items on an empty
page for the moment. But you can override the page--coscev.tpl.php by copying 
it in your theme. views-coscev-view.tpl.php is used to output the style plugin 
can be overriden as well as usual.
