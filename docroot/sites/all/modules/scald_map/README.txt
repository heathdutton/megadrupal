SCALD MAP
---------------------------------

Contents of this file:

 * Introduction
 * Installation

INTRODUCTION
------------

This module introduces a map provider for the scald module.
It can display a predefined address on a map.

It uses the addressfield module to let the user define a postal address.
The address is then geocoded using the geocoder module and the latitude
and longitude coordinates are then stored using the geofield module.
It uses the geofield map module to display the coordinates on a google map.


INSTALLATION
------------

Scald map depends on the following module:

- Scald version 1.2 or higher
- Geofield 2.x branch
- Geocoder
- Addressfield
