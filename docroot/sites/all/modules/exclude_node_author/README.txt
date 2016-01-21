CONTENTS OF THIS FILE
---------------------
* Introduction
* Requirements
* Installation
* Configuration
* Maintainers

INTRODUCTION
------------
Prevents authored on/by information from being displayed on a per node basis. Instead of hiding author information for
an entire content type, can be hidden on just a single node. Designed for sticky nodes (but can be used anywhere)
providing information about a page rather than used as content.

Based on: Exclude Node Title (https://drupal.org/project/exclude_node_title), except instead of excluding the Title, it
excludes the "Submitted by" and "submitted on" information from being displayed on a per node basis.

Provides a checkbox on form node edit, to exclude submitted by/on from display.

REQUIREMENTS
------------
No addition contributed modules required

INSTALLATION
------------
* Install as you would normally install a contributed drupal module. See:
  https://www.drupal.org/documentation/install/modules-themes/modules-7
  for further information.

CONFIGURATION
-------------
* Configure user permissions in Administration » People » Permissions:
  - use exclude node author
    users with this permission will be able to prevent the authored by/on information from being displayed on nodes they
    have create/edit permissions on.
  - administer exclude node author
    Allows users with this permission to modify module settings.
* Configure module settings at /admin/config/content/exclude_node_author
  - Toggle "Do not hide author from users with "Administer exclude node author" permission." on and off.
    Allows author information to be displayed to users with elevated permissions even if the node has the "Exclude
    author information from display" checkbox selected.
  - View a list of node ID's that have been selected to be hidden from display.

MAINTAINERS
-----------
Current maintainers:
* Benjamin Townsend (benjaminarthurt) - https://drupal.org/user/2501220