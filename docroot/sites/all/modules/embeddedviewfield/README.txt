
-- SUMMARY --

EmbeddedViewField is a module that will allow you to render a view as any other
field.

It is based on keithm's ViewField module, except that it will always render the
view, regardless of whether the user is editing or displaying the entity that
has the EmbeddedViewField. The view used for displaying is controlled by the
default value set for the view.

This is useful in situations where you don't want end users to control the
content being displayed by the view, but you still want a flexible way to 
insert content (views with text, images, media etc.) along with the rest of the
fields in the form.

Context, Panels and Blocks could be used to place views and edit forms in the
same page, but not to embed views between specific fields.


Features:
   * Views and displays can be selected using drop down menus.
   * View arguments are supplied through tokens.
   * No code needed to embed a view in an edit form.
   * It allows unlimited number of embeddedviewfields and each one can be
     re-ordered within the form.


This module could be a solution for the following threads:
   * http://drupal.stackexchange.com/questions/111622
   * http://drupal.stackexchange.com/questions/21165
   * https://www.drupal.org/node/48816



For a full description of the module, visit the project page:
  http://drupal.org/project/embeddedviewfield
  
To submit bug reports and feature suggestions, or to track changes:
  http://drupal.org/project/issues/embeddedviewfield


-- REQUIREMENTS --

* Views


-- INSTALLATION --

Install as usual, for further information see: 
https://www.drupal.org/documentation/install/modules-themes/modules-7


-- CONTACT --

Current maintainers:
* Blanca Esqueda (Blanca.Esqueda) - https://www.drupal.org/user/3082389


-- AUTHOR --

Blanca Esqueda (Blanca.Esqueda) - https://www.drupal.org/user/3082389


-- CREDITS & THANKS

*EmbeddedViewField is base on keithm's ViewField module 
 https://www.drupal.org/project/viewfield

*EmbeddedViewField view and display fields are base on Field Embed Views module
 https://www.drupal.org/project/field_embed_views 
