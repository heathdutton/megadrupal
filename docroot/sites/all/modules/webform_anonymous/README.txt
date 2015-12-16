CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Information
 * Installation
 * Support

INTRODUCTION
------------

Current Maintainer: Daniel Imhoff <dwieeb@gmail.com>

Webform Anonymous is a very simple, lightweight module gives the creator of a
webform the option to make the submission results anonymous, by hiding the
username and IP of all the users who submitted the webform.

INFORMATION
-----------

This module adds a simple Yes/No question in the webform form settings to
"Anonymize results." It also adds a checkbox to permanently lock the results
in an anonymous state, so that it would be impossible for anyone to see who
used a webform.

The module is completely compatible with webform. It only anonymizes the user
ID and IP when the submission results are displayed. (This means webform can
still restrict users who have already submitted the form, etc.)

INSTALLATION
------------

1. Copy the entire webform_anonymous directory into sites/all/modules directory.

2. Login as an administrator. Enable the module in the "Administer" -> "Modules"

3. Setup your webforms by editing a webform node and going to the Webform tab,
   and then clicking Form settings, scrolling down to Anonymize results.

SUPPORT
-------

Please use the issue queue for filing bugs with this module at
http://drupal.org/project/issues/1697964
