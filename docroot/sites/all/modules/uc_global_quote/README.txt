-------------------------------
UC global quote Ubercart Module
-------------------------------

Ubercart Shipping quote module that provides custom shipping quotes based on 
configurable weight ranges and custom shipping zones.

Features
--------

Shipping zones could be defined as any combination of countries and regions/states.
You can define multiple rates based on weight ranges for each zone

Dependencies
------------

Ubercart 2.x or 3.x
Drupal 6 - Ubercart 2.x: Requires the ahah helper module: 
 
------------
Installation
------------

Place the entirety of this directory in sites/all/modules, enable the module
Navigate to administer >> build >> modules and enable "Global Shipping Quote"

Add new shipping zones at: admin/store/settings/quotes/methods/zones
Add weight based quotes at: admin/store/settings/quotes/methods/global_quote
Enable the Global quote shipping method at: admin/store/settings/quotes/methods

----------
CHANGELOG
----------

v0.1 Added Regions support. Now it requires http://drupal.org/project/ahah_helper

----
Copyright (c) 2011, Joan Perez i Cauhe
