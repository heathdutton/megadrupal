INTRODUCTION
------------

The Nodequeue Scheduling module extends Nodequeue with scheduling capacities.
Nodes within a nodequeue that have a scheduled publish/unpublish date
in the future can be easily filtered out using Views.

REQUIREMENTS
------------
This module requires the following modules:

* Nodequeue (https://drupal.org/project/nodequeue)
* Chaos tool suite (ctools) (https://drupal.org/project/ctools)

INSTALLATION
------------

* Install as you would normally install a contributed Drupal module. See:
   https://drupal.org/documentation/install/modules-themes/modules-7
   for further information.

CONFIGURATION
-------------

* First, make sure you have at least one nodequeue defined (see Administration » Structure » Nodequeues).

* Configure nodequeues to be extended in Administration » Structure » Nodequeues » Settings.

VIEWS CONFIGURATION
-------------------

* Add the field `Nodequeue: Publish on` as a filter to filter nodes that shouldn't be displayed.

* Make sure the field can be left empty as well, which means there is no scheduling restriction active on that node.


CREDITS
-------

This project has been developed by:
* Marzee Labs, http://marzeelabs.org
* Actency, http://actency.fr
* ARTE GEIE, http://arte.tv
