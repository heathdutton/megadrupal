Drupal activable module:
------------------------
Maintainers:
  Rob Montero (dgo.to/@rmontero / http://drupal.org/user/191552)
Requires - Drupal 7
License - GPL (see LICENSE)

Overview:
--------
Activable helps you build modern UI components quicker and keep your code clean.
Homepage: https://github.com/louisremi/Activable#activable
Download - https://github.com/downloads/louisremi/Activable/activable.zip

Demo:
-----
http://louisremi.github.com/Activable/demo/

Declarative Conventions
-----------------------

Online documentation available at http://louisremi.github.com/Activable/docs/

The Basics

The conventional markup is composed of the data-activable and data-group 
attributes, as well as the active class:

@data-activable is used to indicate that an element should become .active when 
clicked. The value of this attribute indicates the exact behavior of the 
component:
"1" stands for "1 and always 1 is active" and means that when one element is
.active the other elements of the same group lose this class (default value).
"01" stands for "0 or 1 is active" and has the same behavior as `"1"`, but the 
state of an element can be toggled.
"0X" stands for "0 or X are active" and means that state changes don't affect 
the elements of the same group.
@data-group can be used to indicate which group (or component instance) this 
element is part of: every element sharing the same value for this attribute 
will be part of the same group.
.active should be targeted in CSS to render the elements of an activable 
components according to their state.

Installation:
------------
1. Download and unpack the Activable plugin to "sites/all/libraries/activable/".
   Link: https://github.com/downloads/louisremi/Activable/activable.zip
   Drush users can use the command "drush activable-plugin".
2. Download and unpack the Acivable module directory in your modules folder
   (this will usually be "sites/all/modules/").
3. Go to "Administer" -> "Modules" and enable the module.

Drush:
------
A Drush command is provides for easy installation of the Activable plugin 
itself.

% drush activable-plugin

The command will download the plugin and unpack it in "sites/all/libraries".
It is possible to add another path as an option to the command, but not
recommended unless you know what you are doing.

Change Log:
----------
06/13/2012 rmontero Initial commit.

Last updated:
-------------
06/13/2012