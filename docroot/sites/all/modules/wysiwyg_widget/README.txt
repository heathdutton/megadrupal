INTRODUCTION
------------

The Wysiwyg Widget allows javacript & html to be placed 
in a controlled way into the wysiwyg editor. 
NB it can allow ANY html tag to be used by configuring its filter 
to run after filters that clean html such as "Limit allowed HTML tags".

 * For a full description of the module, visit the project page:
   https://drupal.org/project/wysiwyg_widget

 * To submit bug reports and feature suggestions, or to track changes:
   https://drupal.org/project/issues/wysiwyg_widget


REQUIREMENTS
------------
This module requires the following modules:
 * Wysiwyg (https://drupal.org/project/wysiwyg)


INSTALLATION
------------
 * Install as you would normally install a contributed Drupal module. 
   See:
   https://drupal.org/documentation/install/modules-themes/modules-7
   for further information.


CONFIGURATION
-------------
 * Enable the filter and set its order to be last 
   or at least after filters that clean html, 
   such as "Limit allowed HTML tags" 
   for each format widgets are to be used on 
   (/admin/config/content/formats).

 * Enable the "Wysiwyg embed" button in each wysiwyg profile 
   that will have widgets 

HOOK API
--------
 * hook_wysiwyg_widget_embed_alter(&$placeholders)
   Allows custom placeholders to be used for various widgets.
   see wysiwyg_widget.api.php for details.
