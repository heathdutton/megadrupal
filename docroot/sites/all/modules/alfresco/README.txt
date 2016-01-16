
-- SUMMARY --

Alfresco module provides integration between Drupal and Alfresco Enterprise
Content Management System. Alfresco is a Open Source content management
platform for documents, web, records, images, and collaborative content
development.

This module helps you build Drupal websites using the Alfresco's Document
Management repository to store and share the documents.


-- REQUIREMENTS --

* Compatible with Alfresco 3.x or 2.x (Alfresco Enterprise and Community)

* Ext JS 2.x or 3.x (http://www.sencha.com/products/js)
  * Only for Alfresco browser module

* PHP 5.2 or later, with:
  * DOM Extension (part of PHP 5 core)
  * SOAP Extension (--enable-soap)


-- INSTALLATION --

* Note that you will need an installed and configured Alfresco (on remote or
  local server) prior to installing this module.

* Install Alfresco module as usual on your Drupal site.

* Alfresco browser module requires the Ext JS Library. Please download the Ext
  JS library, extract the archive and copy its contents to the following
  location: sites/all/libraries/ext. If you are using Libraries module, you can
  place Ext JS in the Libraries module's search path.


-- CONFIGURATION --

* Configure module settings in Administer >> Site configuration >> Alfresco:

  - Repository: Set URL and credentials for the Alfresco repository.
  
* Configure user permissions in Administer >> User management >> Permissions
  >> alfresco module


-- CREDITS --

Author and maintainer:
* Sergio Martín Morillas <smartinmorillas@gmail.com>
  http://drupal.org/user/191570

Sponsors and thanks:
* The Drupal 6 version has been sponsored by GMV Innovating Solutions.
* Thanks to Manuel Jesús Recena Soto for many suggestions and helpful comments.

* Some of the icons used by this module are part of the Alfresco Community.
