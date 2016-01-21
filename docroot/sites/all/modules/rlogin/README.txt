/* $Id */

-- SUMMARY --

This module provides the standard core user login block in a form that can be incorporated into an external website, for example by using the HTML <object> or <iframe> tags.

It provides a way to assign a cascading style sheet to the form as presented to allow its appearance to fit with the website in which it is incorporated. Already authenticated users are presented with a link to the Drupal site with a user selectable text message.

For a full description of the module, visit the project page:
  http://drupal.org/project/rlogin

To submit bug reports and feature suggestions, or to track changes:
  http://drupal.org/project/issues/rlogin


-- REQUIREMENTS --

Drupal 6


-- INSTALLATION --

* Install as usual, see http://drupal.org/node/70151 for further information.


-- CONFIGURATION --

* Configure operational (administrative settings) in Administer >> Configuration >> People >> Remote Login

  - Customize appearance of the login block by setting the URL of the desired
    cascading style sheet (CSS) file to be used.

  - Customize the text link provided when user is already logged in.


-- CUSTOMIZATION --

* Create a style sheet which can be fetched via URL to make the login form look
  the way you want, e.g. to match the appearance and style of the site it will
  be embedded within.

* Embed the form by using for example, the <iframe> or <object> HTML elements.
  
  Example:

<object id = "rlogin-form" class = "my-class" data = "http://drupal7/rlogin" type="text/html" standby="One moment, loading login form ...">
  Could not load login form.
</object>

* NOTE: Drupal outputs XHMTL, and hence the login form will be XHTML.

-- CONTACT --

Current maintainer and author:
* Matthew Slater - https://drupal.org/user/140053/edit
