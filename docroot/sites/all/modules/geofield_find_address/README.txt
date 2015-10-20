Introduction
-------
The Geofield Find Address module Provides Geofield (Geofield module) widget to
geocode from Address field (Address Field module) and populate Geofield
coordinates fields. Additionally displays google map with selected address on
the node edit page.

Requirements
------------
This module requires the following modules:
1) Address Field (https://www.drupal.org/project/addressfield)
2) Geofield (https://www.drupal.org/project/geofield)

Install
-------
Simply install Geofield Find Address like you would any other module.

1) Copy the geofield_find_address folder to the modules folder in your
installation.

2) Enable the module using Administer -> Modules (/admin/build/modules).

Configuration
-------------
The module has no menu or modifiable settings. There is no configuration.

Use
-------
1) Create at least one Address field (besides your Geofiled).

2) Choose 'Geocode from Postal Address' Geofield widget.

3) Select in the Geofield settings an Address field to geocode from.

On the node edit page you will see a 'Show Location' button under Address field
items.
In case of multiple values you need to add corresponding number of field items
for both fields.
