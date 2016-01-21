getdirections module for Drupal 7.x

If you have any questions or suggestions please contact me at
http://drupal.org/user/52366 or use the Getdirections issue queue.

This module provides a form to make a Google directions map.

Installation
Upload the whole getdirections directory into sites/all/modules/ or
sites/<domain>/modules/ for multisite setups.

Login to Drupal site as a user with administrative rights and go to
Administer > Site building > Modules. Scroll down to Other modules section, you
should now see Getdirections module listed there. Tick it and save configuration.

After installation go to Administer > Site configuration > Get directions and
select your preferences and Save Configuration.

If you have the gmap module installed and configured, getdirections will
use its configuration as a starting point.

Also remember to go to Administer > User management > Permissions and
set up permissions according to your needs.

If you have the location or getlocations_fields module installed you can
make use of any nodes containing location information.

For instance, if you want to "preload" the getdirections form with information
about the destination use a URL in this format:

getdirections/location/to/99

Where '99' is the node id (nid) of the location.
The user will only have to fill in the starting point.

To do it the other way around use
getdirections/location/from/99

You can also get a map with waypoints
getdirections/locations_via/1,2,3,99

If you have both the starting point and destination node ids then you can use
getdirections/locations/1/99

Where '1' is the starting point node id and 99 is the destination node id
(note the 's' in locations)

If you are using the location_user or getlocations_fields module you can
make a link to a user's location with the user id:
getdirections/location_user/to/66

Same goes for 'from'

You can also get a map with waypoints of user locations with the user ids:
getdirections/locations_user_via/1,2,3,99

You can mix nodes and users with
getdirections/location_u2n/66/99
Where 66 is is user id, 99 is node id

and vice-versa
getdirections/location_n2u/99/66
Where 66 is is user id, 99 is node id

You can build a map by supplying latitude/longitude pairs delimited with a '|'
eg getdirections/latlons/nn,nn|nn,nn|nn,nn
where nn,nn is a latitude/longitude pair.
A maximum of 8 pairs is permitted by Google unless you have a commercial license.

If you have the Colorbox module installed and enabled in Get Directions
you can place any of the above paths in a colorbox iframe by replacing
'getdirections' with 'getdirections_box'.
To enable this for a link you can use the 'colorbox-load' method,
make sure that this feature has been enabled in colorbox
and use a url like this:
<a href="/getdirections_box?width=700&height=600&iframe=true" class="colorbox-load">See map</a>

or (advanced use) by adding rel="getdirectionsbox" to the url, eg
<a href="/getdirections_box" rel="getdirectionsbox">See map</a>

The last method uses the settings in admin/settings/getdirections for colorbox
and uses its own colorbox event handler, see getdirections.js. You can define your own
event handlers in your theme's javascript.
'getdirections_box' has it's own template, getdirections_box.tpl.php which can be
copied over to your theme's folder and tweaked there.

If you install
http://drupal.org/project/menu_attributes
you can get colorbox links into the menu system.
for instance, you can put
getdirections_box?width=700&height=600&iframe=true
in the path and
colorbox-load
in the class textfield
or if you have colorbox enabled in Get Directions:
getdirections_box
in the path and
getdirectionsbox
in the Relationship textfield.

------------------
Get Directions API
These functions are for use in other modules.
There are functions available to generate these paths:

getdirections_location_path($direction, $nid)
Where $direction is either 'to' or 'from' and $nid is the node id concerned.

getdirections_locations_path($fromnid, $tonid)
Where $fromnid is the starting point and $tonid is the destination

getdirections_locations_via_path($nids)
Where $nids is a comma delimited list, eg "1,2,3,99" of node ids
Google imposes a limit of 25 points

If you have the latitude and longitude of the start and end points you can use

getdirections_locations_bylatlon($fromlocs, $fromlatlon, $tolocs, $tolatlon)
Where $fromlocs is a description of the starting point,
$fromlatlon is the latitude,longitude of the starting point
and $tolocs and $tolatlon are the same thing for the endpoint

See getdirections.api.inc for more detail.

If you are using Views and Location it will provide a 'Get driving directions' block
when you are viewing a location node or a user with a location.

Getdirections now supports the Googlemaps API version 3, this has many new features.


The getdirections_latlon() function can be used in code or called by URL.
getdirections_latlon($direction, $latlon, $locs)
Where $direction is either 'to' or 'from', $latlon is the latitude,longitude to be used
and $locs is an optional string to describe the point being used.
The URL method would be in the form of
<a href="/getdirections/latlon/(from or to)/(lat,lon)/optional description">My Link</a>
You can also use the function getdirections_location_latlon_path() in getdirections.api.inc to
generate the string for you.

Themeing
To change the way the map is displayed you should copy the theme function you want to change
to your theme's template.php, renaming appropriately and editing it there.

There is plenty of help on themeing on drupal.org

An example of a theming change, from http://drupal.org/node/1113042

Your theme should have a template.php,
copy the function theme_getdirections_show() from getdirections.module to your template.php,
renaming it appropriately, eg from
theme_getdirections_show
to
mythemename_getdirections_show
where 'mythemename' is the name of your theme. Then change the following:

  $rows[] = array(
    array(
      'data' => '<div id="getdirections_map_canvas_' . $mapid . '" style="width: ' . $width . '; height: ' . $height . '" ></div>',
      'valign' => 'top',
      'align' => 'center',
      'class' => array('getdirections-map'),
    ),
    array(
      'data' => ($getdirections_defaults['use_advanced'] && $getdirections_defaults['advanced_alternate'] ? '<button id="getdirections-undo-' . $mapid . '">' . t('Undo') . '</button>' : '') . '<div id="getdirections_directions_' . $mapid . '"></div>',
      'valign' => 'top' ,
      'align' => 'left',
      'class' => array('getdirections-list'),
    ),
  );

to
  $rows[] = array(
    array(
      'data' => '<div id="getdirections_map_canvas_' . $mapid . '" style="width: ' . $width . '; height: ' . $height . '" ></div>',
      'valign' => 'top',
      'align' => 'center',
      'class' => array('getdirections-map'),
    ),
  );
  $rows[] = array(
    array(
      'data' => ($getdirections_defaults['use_advanced'] && $getdirections_defaults['advanced_alternate'] ? '<button id="getdirections-undo-' . $mapid . '">' . t('Undo') . '</button>' : '') . '<div id="getdirections_directions_' . $mapid . '"></div>',
      'valign' => 'top' ,
      'align' => 'left',
      'class' => array('getdirections-list'),
    ),
  );

What this does is rearrange the table so that it produces two rows with one datacell each instead of one row with two datacells.

Make sure you flush the theme registry when you have made the changes, the devel and admin_menu modules are helpful for this.

Putting Getdirections map and list into divs. This example should work with responsive themes.
replace everything from
  $header = array();
to
  $output .= '<div class="getdirections">' . theme('table', array('header' => $header, 'rows' => $rows)) . '</div>';

with
  $output .= '<div class="getdirections">';
  $output .= '<div class="getdirections-map"  style="width: ' . $width . '; height: ' . $height . '" >';
  $output .= '<div id="getdirections_map_canvas_' . $mapid . '" style="width: 100%; height: 100%" ></div>';
  $output .= '</div>';
  $output .= '<div class="getdirections-list">';
  if ($getdirections_defaults['use_advanced'] && $getdirections_defaults['advanced_alternate']) {
    $output .= '<button id="getdirections-undo-' . $mapid . '">' . t('Undo') . '</button>';
  }
  $output .= '<div id="getdirections_directions_' . $mapid . '"></div>';
  $output .= '</div>';
  $output .= '</div>';

Blocks.
Here is a simple way to crete a block with a form and map in it. Ensure you have php filter enabled and add
<?php
if (arg(0) == 'node') {
  $nid = arg(1);
  if (is_numeric($nid) && $nid > 0) {
    echo getdirections_setlocation('to', $nid);
  }
}
?>
This will only show on node view.

see http://drupal.org/node/880332

Adding a map directly into a block:
See http://drupal.org/node/1376392#comment-7135346

Create a custom block. Use the php filter for the content. Then paste this into your block content:

<?php
$n = FALSE;
$nid = FALSE;
if (arg(0)) {
  $n = arg(0);
}
if (arg(1)) {
$nid = arg(1);
}
if ($n && $nid && is_numeric($nid) && $nid > 0) {
  $l = getdirections_load_locations($nid, $n);
  if ($l) {
    echo getdirections_entity_setlocation($n ,'to', $nid);
  }
}
?>

