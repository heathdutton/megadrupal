
SUMMARY
=======

A collection of developer tools for quick Google map creation from Drupal code.
Uses google map API version 3.

This is utility module - you need this module only if some other module requires
it.

Main goal of this module is to offer developers very light and easy PHP
implementation of Google Map API version 3 for Drupal.


REQUIREMENTS
============

None.


INSTALLATION
============

Download module and place it in your contributed module folder. There is no need
to enable this module but you will need to clear module list cache so Drupal
can pick path of gmap3_tools module - visit admin module list page for this.


HOW TO USE IT
=============

Check example page for couple of short examples
http://your-site/gmap3_tools/example.

Bute basically you need to do 3 things:

1) Load gmap3_tools.inc file:

  <?php module_load_include('inc', 'gmap3_tools'); ?>

2) Call gmap3_tools_add_map() function from API file with appropriate map
   configuratio array:

  <?php
    // Map example with some custom options and 2 markers.
    gmap3_tools_add_map(array(
      'mapId' => 'map-canvas',
      'mapOptions' => array(
        'centerX' => -34.397,
        'centerY' => 150.644,
        'zoom' => 8,
      ),
      'markers' => array(
        gmap3_tools_create_marker(-35, 150, 'Marker with info window', 'Marker info window content.'),
        gmap3_tools_create_marker(-35, 150.5, 'Marker without info window'),
      ),
      'gmap3ToolsOptions' => array(
        'defaultMarkersPosition' => GMAP3_TOOLS_DEFAULT_MARKERS_POSITION_CENTER,
      ),
    ));
  ?>

  This function will initialize needed js code and create google map js
  definition for you.

3) Last thing you need to do is add html element with 'gmap-canvas' in your
   Drupal output - this element will hold Google map:

   <?php $page_putput .= '<div id="gmap-canvas" style="width:500px; height:400px"></div>; ?>


From API version 3 you do not need to have Google API key in order to use Google
Map API, but if you need to do that you can change value of gmap3_tools_api_key
variable value.

  <?php variable_get('gmap3_tools_api_key', 'YOUR_GOOGLE_API_KEY')); ?>

For a list of all supported configuration options check $defaults array
definition in gmap3_tools_add_map() function. But in short next features are
supported for now:

* Map creation with custom center and custom zoom level.
* Markers with custom icon, title. MarkerOptions object is fully supported on
  global level (for all markers) and locally per each marker. Check example 3
  in gmap3_tools module code for example.
* Infoboxes for markers with custom content.
* Option for auto centering and zooming of added markers on maps.

If you need to access gmaps objects from your custom js code, you can do so like
this:

  // Get gmap object from it canvas.
  var gmap = $('gmap-canvas').data('gmap');

You can also access gmap markers array from your custom js code like this:

  // Get gmap markers array from it canvas.
  // Do something with map marker.
  var gmapMarkers = $('gmap-canvas').data('gmapMarkers');
  for (var i = 0; i < gmapMarkers.length; i++) {
    var marker = gmapMarkers[i];
  }


CONTACT
=======

Current maintainers:

* Puljic Ivica (pivica) - http://drupal.org/user/41070
* Vladimir Mitrovic (devlada) - http://drupal.org/user/1663518
