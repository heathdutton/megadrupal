GENERAL
-------------
This module makes it easy to clone products by simply pre-populating the product
creation form with all of the data from an existing product. Someone will eventually
come out with an entity_clone module that will make this obsolete I'm sure. I built
this to take care of some immediate needs.

This module is very similar to node_clone (clone). I started from the ground
up and implemented the different pieces from that module that applied to
what we're trying to achieve here. Regardless of how different or similar this
module is, I am thankful for the efforts of the node_clone authors.

INSTALLATION
-------------
Just enable the module (and be sure to read the Configuration section below)

CONFIGURATION
-------------
Permissions:
  This modules creates two new permissions (Clone any product, Clone own products)
  Please set access to these permissions accordingly.

Product Types:
  You can choose which content types you do NOT want to allow users to clone.
  You will find this configuration page under Store > Configuration > Clone settings.
