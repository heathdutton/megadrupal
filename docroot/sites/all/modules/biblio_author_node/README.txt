CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Recommended modules
 * Technical information
 * Installation
 * Configuration

INTRODUCTION
------------

This module adds an option to Biblio module that lets you link Biblio authors to
nodes. By default, Biblio module only allows you to link authors to user
profiles or Biblio author pages.

RECOMMENDED MODULES
-------------------

 * Chosen (https://www.drupal.org/project/chosen):
   It just makes select form items far more usable!

TECHNICAL INFORMATION
---------------------

 * This module works on 7.x-1.0-rc7 and up. So far it has been tested on
   7.x-1.0-rc7.

 * This module is not compatible with 7.x-2.x branch of Biblio module.

 * This module adds field "drupal_nid" to table biblio_contributor_data to be
   able to link authors to nodes.

 * This module makes the new field available on views.
 
 * Notice to all users of dev version of 2015-12-01: You will have to reenable
   node link checkbox and reselect your target node bundle (due to variable
   rename) Sorry for the inconveniences.

INSTALLATION
------------

 * Install as you would normally install a contributed Drupal module. See:
   https://drupal.org/documentation/install/modules-themes/modules-7
   for further information.

CONFIGURATION
-------------

 * Configure module's options in Administration » Configuration »
   Content authoring » Biblio settings » Preferences:

    - Selet "Links" vertical tab at the left of the screen

      Scroll down and find "Hyperlink author names to a node (Biblio author node)"
      option. Just enable it. Make sure "Hyperlink author names" is enabled too.
      Moreover, you have to select a target node bundle.

 * Configure Biblio authors Drupal Node ID in Administration » Configuration »
   Content authoring » Biblio settings » Authors:

   - Edit an author you want to link to a node page
   
     Scroll down and find "Drupal Node ID" field. Select your desired
