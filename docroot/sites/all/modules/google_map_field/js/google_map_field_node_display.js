
/**
 * @file
 * This file contains the javascript functions used to display a map when the
 * entity it is attached to is displayed
 */

/**
 * Declare global variable by which to identify the map.
 */
var google_map_field_map;

/**
 * Add code to generate the map on page load.
 */
(function ($) {
  Drupal.behaviors.google_map_field = {
    attach: function (context) {
      googleMapFieldNodeDisplay();
    }
  };

  googleMapFieldNodeDisplay = function() {
    $(".google_map_field_display").each(function(index, item) {

      // Get the settings for the map from the Drupal.settings object.
      var lat = $(this).attr('data-lat');
      var lon = $(this).attr('data-lng');
      var zoom = parseInt($(this).attr('data-zoom'));
      var S = Drupal.settings.googleMapField;

      // Create the map coords and map options.
      var latlng = new google.maps.LatLng(lat, lon);
      var mapOptions = {
        zoom: zoom,
        center: latlng,
        disableDoubleClickZoom: S.disableDoubleClickZoom,
        draggable: S.draggable,
        keyboardShortcuts: S.keyboardShortcuts,
        noClear: S.noClear,
        panControl: S.panControl,
        rotateControl: S.rotateControl,
        scaleControl: S.scaleControl,
        scrollwheel: S.scrollwheel,
        streetViewControl: S.streetViewControl,
        zoomControl: S.zoomControl,
        mapTypeId: google.maps.MapTypeId.ROADMAP
      };
      google_map_field_map = new google.maps.Map(this, mapOptions);

      google.maps.event.trigger(google_map_field_map, 'resize')

      // Drop a marker at the specified position.
      marker = new google.maps.Marker({
        position: latlng,
        optimized: false,
        map: google_map_field_map
      });

    });
  }

})(jQuery);
