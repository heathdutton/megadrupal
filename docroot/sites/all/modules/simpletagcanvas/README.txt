********************************************************************
                      D R U P A L    M O D U L E
********************************************************************
Name: SimpleTagCanvas
Author: Andre Langner (medienverbinder)
Contact: http://drupal.org/user/1293712
Drupal: 7
RELEASE STATUS: This module is at an initial release state.
********************************************************************

INTRODUCTION
------------

With this module you can display a block with manually specified
graphics and text links within the plugin "TagCanvas" cloud shapes.
It is primarily used for manually defined graphics and is not a
substitute for the taxonomy controlled module TagCanvas.The ability
to create text links is only a option to offer a combination of text
and graphics. All links, images and options should be adjusted easily
in the backend.


INSTALLATION
------------

Libraries API 2

Download and install the Libraries API 2 module.
Move the downloaded module package to sites/all/modules/.
  => sites/all/modules/libraries

In administration back-end on the module configuration (admin/modules)
page you have now to install/activate the module and save the module
configuration.


TagCanvas PlugIn

Download the TagCanvas plugin.
Make sure to use the TagCanvas plugin version 2.1.2.

Move the plugin to the directory "tagcanvas" and place it inside 
  => "sites/all/libraries"

Make sure the path to the plugin file becomes
  => "sites/all/libraries/tagcanvas/jquery.tagcanvas.min.js"


SimpleTagCanvas

Download and install the SimpleTagCanvas module.
 Move the downloaded module package to:
=> sites/all/modules/simpletagcanvas

or

=> sites/all/modules/custom/simpletagcanvas
(yeah ! its custom ;-) )

In administration back-end on the module configuration (admin/modules)
page you have now to install/activate the module and save the module
configuration.


Notice
The Drupal status report provides information on the correct installation
of the TagCloud plugin. (/admin/reports/status)


Activating and placing the block

After activation of the module a new block is added to the block configuration
page under structure / blocks. Now move the new block to the desired region and
make some block specific configuration.


Configuration

After installation you should check the configuration page for the
SimpleTagCanvas module and customize the settings.
(/admin/config/content/simpletagcanvas/)



CONTACT
-------

Current maintainer:
Andre Langner (medienverbinder) - http://drupal.org/user/1293712
