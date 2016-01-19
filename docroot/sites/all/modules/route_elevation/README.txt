
ABOUT Route Elevation
---------------------
The intent of this module is to render a block with an elevation profile
retrieved by the GPX data of a file belonging to the node. Optionally, it
also shows a map with the GPX route.

USAGE
-----
After installing and enabling the module,
1) Create a node with a file field to store the GPX (e.g., field_gpx)
2) Configure Route Elevation settings (/admin/config/services/route_elevation)
   to link the rendered block to the GPX field
3) Show the block (Route Elevation> Route Elevation Block) in the node page 
   (e.g., exploiting the "context" module).

The Google Elevation API
------------------------
The module uses the Google Elevation API, 
  (https://developers.google.com/maps/documentation/elevation/)
taking into account its current limitations, in particular sampling the number 
of points to less than 512 (or the desired amount) locations per request.

Usage Limits for the Users of the free API (see the API page for updates):
- 2,500 requests per 24 hour period.
- 512 locations per request.
- 25,000 total locations per 24 hour period.

Notes
-----
This module builds a legend with partial distances in X Axis and
corresponding heights in Y Axis. Inside the code I also provide a hack to
retrieve the total distance from a Drupal field in the node, in case
it is hand reported (see js/google_elevation.js:109, 124).
This because, in such case, the calculated distance could be different and
lead to a disparity with the legend.
To use it just uncomment and edit the code included between //-{ ... //-}.
