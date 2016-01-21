CONTENTS OF THIS FILE
---------------------
* Introduction
* Requirements
* Installation
* Configuration
* Troubleshooting
* FAQ
* Maintainers

INTRODUCTION
------------
The Limit Visit module allows a site manager to limit the amount of times a
user can view a page or set of pages per hour. The motivation behind this
is for something like a directory that needs to be protected against someone
signing into the directory and scraping it for everyone's personal information.
This will deter scraping by forcing them to wait between so many views.

REQUIREMENTS
------------
Just Drupal 7 core

INSTALLATION
------------
* Install as you would normally install a contributed Drupal module. See:
  https://drupal.org/documentation/install/modules-themes/modules-7
  for further information.

CONFIGURATION
-------------
* Configure user permissions in Administration > People > Permissions:
  - Administer Limit Visit
    Will allow you to set which pages are limited and where users should be
    redirected when they reach the limit
  - By Pass Limit Visit
    Is for users who should not be stopped by limit visit.
* Configure the Limit Visit settings in Administration > Configuration >
  System > Limit Visit.
* The path you enter can include wild cards (*)

TROUBLESHOOTING
---------------
* As soon as I start running into trouble with it I will start shooting it
FAQ
---
Q: Why aren't there any FAQ
A: No one has frequently asked me questions about this yet.

MAINTAINERS
-----------
Current maintainers:
* Bryan Heisler (geekygnr) - https://drupal.org/user/2775199
Written on behalf of the:
  University of Waterloo
  Department of Advancment
  http://uwaterloo.ca/support
