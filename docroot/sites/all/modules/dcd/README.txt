CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Installation
 * Configuration


INTRODUCTION
------------

Current Maintainer: Viswanath Polaki <polaki_2005@yahoo.com>

Delete content directly module helps users to delete their content without
filling required fields in the content edit form.

Any user who has "delete content directly" or "delete content" permissions can
view Delete Content link in the content edit form.

Case where this module will be very useful is as follows:
During site building when content fields are increased or changed from non
required field to required field, many sites who already have created their
content have to refill their required content fields to delete content in
content edit form. This is default functionality of deleting content in drupal.

But as the user is deleting the content there is no sense in asking user to
fill the required content fields before deleting the content. He should be able
to delete content directly.

This module removes the delete content button and places a delete content link
which takes user directly to confirm delete page.


INSTALLATION
------------

* Install as you would normally install a contributed Drupal module. See:
   https://drupal.org/documentation/install/modules-themes/modules-7
   for further information.


CONFIGURATION
-------------
Based on "delete content directly" or "delete content" permissions the
content edit form is displayed replacing native delete content button.
