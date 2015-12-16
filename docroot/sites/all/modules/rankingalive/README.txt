CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Requirements
 * Installation
 * Custom tracking configuration
 * User permissions


INTRODUCTION
------------

 Current Maintainer:
 * Julien Dubreuil (JulienD) - http://drupal.org/user/519520

 This module provides integration with Ranking Alive website.


REQUIREMENTS
------------

 Ranking Alive website account (http://report.rankingalive.com)


INSTALLATION
------------

 1. Download and extract the module's tarball (*.tar.gz archive file) into
    your Drupal site's contributed/custom modules directory:

      /sites/all/modules

 2. Enable the module from the site's module page:

      Administration > Modules

 3. Configure the module in the settings page and enter your Ranking Alive ID:

      Administration > Configuration > System > Ranking Alive


CUSTOM TRACKING CONFIGURATION
-----------------------------

 Custom tracking configurations allow administrators to add or remove the
 tracking

 1. Go to Rankingalive settings configuration:

      Administration > Configuration > System > Ranking Alive

 2. Click on the "Show Pages tracking settings" fieldset

 3. Manage the custom pages settings

      The "Page tracking settings" area is a copy of the Drupal's block
      visibility settings.
      The default is set to "Add to every page except the listed pages".
      By default the following pages are listed for exclusion:
        - admin
        - admin/*
        - batch
        - node/add*
        - node/*/*
        - user/*/*

      These defaults are changeable by the website administrator or any other
      user with 'administer rankingalive' permission.

      Like the blocks visibility settings in Drupal core, there is now a choice
      for "Add if the following PHP code returns TRUE." Sample PHP snippets that
      can be used in this textarea can be found on the handbook page
      "Overview-approach to block visibility" at http://drupal.org/node/64135.


USER PERMISSIONS
----------------

 The module provides user/role permission which can be granted at:

      Administration > People > Permissions
