
CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Features
 * Requirements
 * Installation
 * Configuration
 * For More Information


INTRODUCTION
------------

Salsa Entity is a suite of modules that integrates the Salsa CRM from Salsa
Labs (http://www.salsalabs.com) with Drupal. It exposes the various Salsa
objects as entities within Drupal, which allows close integration with other
Drupal modules such as Rules and Entity Reference.

The goal is to replace Salsa's public user interface completely, integrating
Salsa objects within a Drupal website, and allowing customizations through the
standard Drupal APIs and concepts (e.g. hook_form_alter()).

Salsa Entity and its submodules expose a growing list of Salsa objects within
Drupal, including at this writing:

 * Supporters
 * Groups
 * Signup Pages
 * Donate Pages
 * Questionnaires
 * Tell-a-Friend Pages
 * Events
 * Petitions
 * Targeted Actions
 * Unsubscribe Pages


FEATURES
--------

 * Salsa pages are displayed as Drupal forms which are then submitted to Drupal
 and the necessary actions are made through the Salsa API

 * All fields, including custom fields, are exposed as Entity properties

 * Rules integration

 * Entity reference integration, including support for the Autocomplete widget

 * Self-management, including profile information, donations, peer-to-peer
 donation pages, and events. Supporters are found based on the email address and
 connected through a user field, support for back-referencing the supporter to
 the Drupal UID through a Salsa custom field.

 * Similar to the Salsa frontend, the supporter values of a form are kept in the
 session and other forms are automatically pre-filled.

 * Basic views integration is provided through EntityFieldQuery Views. This
 allows listing, sorting, and filtering of Salsa data such as supporters,
 events, etc.


REQUIREMENTS
------------

* Drupal 7. There will be no D6 backport.
* Entity API
* Salsa API
* A Salsa campaign manager login

INSTALLATION
------------

After installing and configuring the Salsa API module, install and enable
Salsa Entity. See http://drupal.org/documentation/install/modules-themes/modules-7
for more information on installing modules in Drupal 7.

After enabling the main Salsa Entity module, enable the submodule for each
Salsa object you wish to expose to Drupal. See the README.txt files in each
submodule directory for additional information.


CONFIGURATION
-------------

Configure Salsa Entity and its submodules at Configuration >> Web services >>
Salsa.


FOR MORE INFORMATION
--------------------

 * Project Page: http://drupal.org/project/salsa_entity
 * Issue Queue: http://drupal.org/project/issues/salsa_entity