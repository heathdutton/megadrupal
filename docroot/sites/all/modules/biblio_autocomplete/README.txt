
This module extends the functionality of the biblio module:
https://drupal.org/project/biblio

This module provides functionality for auto-completion of fields in the Biblio
node type using both previously entered values and third party services
(currently two external plugin modules are included: IPNI and ZooBank).

The module is designed to enable multiple plugins to contribute to the
autocomplete selection list (e.g. results can be simultaneously retrieved from
both IPNI and ZooBank). An API is provided for developers to write additional
plugins.

The following plugins are included as part of this package (you will need to
enable at least one plugin module to use this module):

* Biblio Self Autocomplete
Allows you to autocomplete on fields using values that you have previously
entered in other Biblio nodes.

* International Plant Names Index (IPNI) – http://ipni.org
Using this plugin it is possibly to autocomplete any title field (e.g. node
title, alternate title, ..) using an autocomplete based on the IPNI webservice.

* ZooBank – http://zoobank.org
This plugin allows you to autocomplete fields from ZooBank - intended to be the
official registry of animal names.


Security Note
-------------
To avoid problems with any visitor being able to perform queries against the
AJAX callback, thus possibly bypassing content permissions and accessing private
content, the "Access to Biblio Autocomplete results" (aka "access biblio
autocomplete") permission was added. This permission should only be given to
roles that will have "edit" or "create" access to the Biblio content types and
specifically should not be given to the "anonymous visitor" role.
