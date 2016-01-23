CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Configuration
 * Maintainers

INTRODUCTION
------------

This module help to show/hide country specific Node's.
This module requires following modules-
1) IP2Country
2) Locale
3) Field
4) List

Functionally it detects User's IP address.
Identifies visitor's country to implement node access restriction.

This module helps to create country specific Node's i.e.
Node will be hidden for the selected countries.
It detects gets User's country from Ip2Country information and based on this
it shows/hides nodes.

CONFIGURATION
-------------

Installation and configuration:
1) Download and enable module.
2) Go to Structure -> Country Specific Nodes to attach country field to listed
   content types.
3) Go to Structure -> Country Specific Nodes -> Set Default Country to specify
   the default/fallback country if the users IP has not been detected.

Note:
This module denies access to node based on the countries selected in the
node edit form.
It does not hide the displaying/rendering of nodes in listing e.g. Drupal's
front page listing of nodes, instead it provides a $node->csn_hidden value to
the template and in node_load object to determine whether to display the
nodes at template level ot any other custom rendering of nodes.
$node->csn_hidden contains 1 for hidden nodes and 0 for non hidden nodes.


MAINTAINERS
-----------
Makarand Chavan
https://www.drupal.org/u/cmak

Vikrant Ramteke
https://www.drupal.org/u/vikrant-ramteke

Sandeep Kumbhatil
https://www.drupal.org/u/sandeep.kumbhatil
