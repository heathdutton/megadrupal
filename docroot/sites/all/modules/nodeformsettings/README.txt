CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Requirements
 * Installation
 * Configuration
 * Troubleshooting


INTRODUCTION
------------
This module lets you change several 'small' behaviors with hook_form_alter,
in both nodes and comments, doing it per content type so you can configure
different aspects of your nodes/comments.


REQUIREMENTS
------------
Drupal 7.x
Comment, Text, Field, Field SQL storage (for the Comment Form Settings module
only)


INSTALLATION
------------
 * To install the module copy the 'nodeformsettings' folder to your
   sites/all/modules directory.
 * Go to admin/modules. Enable the module(s). Depending on your
   needs you can activate only one or both modules: "Node Form Settings", "Comment Form Settings".
Read more about installing modules at http://drupal.org/node/70151


CONFIGURATION
-------------
1. Go to admin/structure/types
2. Click on the "Edit" link beside the appropriate content type.
3. Scroll down the page. Click on "Node form settings" or "Comment form
   settings".
4. Follow instructions on screen.
5. Click on "Save content type" button.


TROUBLESHOOTING
---------------
About upgrading read more at http://drupal.org/node/250790
