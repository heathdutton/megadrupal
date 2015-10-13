-- SUMMARY --

Route Planner is a very simple and lightweight Module to create a route from
any address to a point of interest (i.e. your company location).
The Module works with Google Maps and will provide two blocks for you, 
find the block settings under admin/structure/blocks:
* Block 1 - Route Planner Address Field
  This block will show a input form for your starting point of the route
  and displays the driving time and the total distance.
* Block 2 - Route Planner Map Display
  This block contains the Google Map - Display with your point of interest
  or your calculated route to the POI.


You can also use the Map output in your own content (i.e. Nodes or Blocks),
just enable the Route Planner Address Field Block on this site and put this
"<div id="map_canvas" style="height:300px"></div>"
in your Full HTML - Content Area.


-- REQUIREMENTS --

This Module will only work when JavaScript is enabled and the
"Route Planner Address Field"-Block is on the desired page active.

This module will load the Google Maps API V3.

-- INSTALLATION --

* Install as usual, see http://drupal.org/node/70151 for further information.


-- CONFIGURATION --

* You can configure some settings in Administration � Config � Route Planner


