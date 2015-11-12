
/**
 * @file
 * This file contains the javascript functions used to display a map when the
 * entity it is attached to is displayed.
 */

/**
 * Declare global variable by which to identify the map.
 */
var baidumap_field_map;

/**
 * Add code to generate the map on page load.
 */
(function ($) {
  Drupal.behaviors.baidumap_field = {
    attach: function (context) {
      // Pick up all elements of class baidumap_field and loop
      $(".baidumap_field_display").each(function(index, item) {
        var objId = $(item).attr('id');
        baidumap_field_load_map(objId);
      });
    }
  };
})(jQuery);

/**
 * This function is called by the baidumap_field Drupal.behaviour and
 * loads a baidu map in tot he given map ID container.
 */
function baidumap_field_load_map(map_id) {
  // Get the settings for the map from the Drupal.settings object.
  var lat = Drupal.settings.gmf_node_display[map_id]['lat'];
  var lng = Drupal.settings.gmf_node_display[map_id]['lng'];
  var zoom = parseInt(Drupal.settings.gmf_node_display[map_id]['zoom']);
  // Create the map coords and map options.
  var map = new BMap.Map(map_id);
  var point = new BMap.Point(lng, lat);
  map.centerAndZoom(point, zoom);
  // Create marker.
  var marker = new BMap.Marker(point);
  map.addOverlay(marker);
  // Add control bar.
  var opts = {anchor: BMAP_ANCHOR_TOP_LEFT, type: BMAP_NAVIGATION_CONTROL_SMALL}
  map.addControl(new BMap.NavigationControl(opts));
  map.enableScrollWheelZoom();
}
