Drupal Pines Notify module:
------------------------
Maintainers:
  Zak Huber (http://drupal.org/user/1437276)
Requires - Drupal 7
License - GPL (see LICENSE)


Overview:
--------
PNotify (formerly Pines Notify) is a JavaScript notification plugin, developed by Hunter Perrin as
part of the Pines framework. It is designed to provide an unparalleled level of
flexibility, while still being very easy to implement and use.

http://pinesframework.org/pnotify/


Installation:
------------
1. Download and unpack the Libraries module directory in your modules folder
   (this will usually be "sites/all/modules/").
   Link: http://drupal.org/project/libraries
2. Download and unpack the Pines Notifiy module directory in your modules folder
   (this will usually be "sites/all/modules/").
3. Download and unpack the Pines Notify plugin in "sites/all/libraries".
    Make sure the path to the plugin file becomes:
    "sites/all/libraries/pnotify/pnotify.core.js"
   Link: https://github.com/sciactive/pnotify/archive/master.zip
   Drush users can use the command "drush pnotify-plugin".
4. Go to "Administer" -> "Modules" and enable the Pines Notify module.


Configuration:
-------------
Go to "Configuration" -> "User Interface" -> "Pines Notify" to find
all the configuration options.


Drush:
------
A Drush command is provides for easy installation of the Pines Notify
plugin itself.

% drush pnotify-plugin

The command will download the plugin and unpack it in "sites/all/libraries".
It is possible to add another path as an option to the command, but not
recommended unless you know what you are doing.
