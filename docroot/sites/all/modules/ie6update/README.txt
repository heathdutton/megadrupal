
------------------
IE 6 UPDATE MODULE
------------------

SUMMARY
-------

This module integrates the IE6 Update JavaScript file with Drupal, unobtrusively
encouraging site users to upgrade their browsers.


FEATURES
--------

* Mimics the IE information bar to suggest to the user to upgrade their browser.
* Links to the IE update page so the user can upgrade their browser.
* The message in the information bar is configurable.
* The page linked to from the information bar is configurable.


INSTALLATION
------------

* Enable the module in the usual Drupal way.
* Follow translation instructions below if appropriate.


HOW TO TRANSLATE
----------------

Note that Drupal 6 and Drupal 7 handle variable translation differently. Please
see the relevant section below.

* DRUPAL 7

  You can translate the message in the information bar and link to a non-English
  version of IE if you have the Variable and i18n modules installed. You will
  also need i18n's i18n_variables (Multilingual Variables) and its dependencies.

  Once you've downloaded and installed the above modules, visit
  Configuration > Multilingual Settings > Variables. 

  Find the "Destination URL" and "Update Message" variables, select them to be
  translated and save the configuration. Now, if you return to the IE6 Update
  administration page, you should see messaging on how to translate the message
  and URL.

  For more information on Drupa 7 variable translation, please see: 
  http://hojtsy.hu/blog/2011-feb-25/drupal-7039s-new-multilingual-systems-part-5-site-settings


* DRUPAL 6

  You can translate the message in the information bar and link to a non-English
  version of IE if you have the i18n module installed.

  To do this, add the following variables to the $conf['i18n_variables'] array
  in your settings.php file:

  * ie6update_destination_url
  * ie6update_update_bar_message

  Once this is complete, switch to each language and visit the ie6update
  settings page using the appropriate language version of the site, enter an 
  appropriatetranslation, and save the page. The settings for each language will
  then be saved independently.

  For more information on i18n variables and translations, please see
  http://drupal.org/node/313272
