// $Id $

Description
-------
The Evanced Registration module is used to allow users to register for events on your Evanced event site without leaving the context of your Drupal site.

This module is maintained by the Virtual Services team at Richland Library (http://www.myrcpl.com) in South Carolina.
The Drupal 7 version is maintained by Mark W. Jarrell (http://drupal.org/user/249768).

Install
-------
Installing the module is simple:

1) Copy the evanced_registration folder to the modules folder in your installation.

2) Enable the module using the Modules administration page (/admin/modules).

3) Give your administrators the ability to "Administer Evanced Registrtation" by going to the Permissions page (/admin/people/permissions).

4) Configure settings for the module on the settings page (/admin/config/development/evanced-registration).


Creating Links to Register for Events
-------
In most situations, your events will already exist on your site and have likely been imported via the Evanced Events Importer module
(http://drupal.org/project/evanced_events_importer).
Once you have the events appearing, you'll need to somehow create a link on your event node pages (via theming etc.) that points to the registration URL for
that particular Evanced event ID.
The links will look like /evanced-registration/1234 (with 1234 being the Evanced ID of that event, not to be confused with the node ID of the Drupal node).


Support
-------
If you experience a problem with the module, file a
request or issue on the Evanced Registration issue queue (http://drupal.org/project/issues/evanced_registration).
DO NOT POST IN THE FORUMS. Posting in the issue queue is a direct line of
communication with the module author.