/**
 * @file
 * Drupal integration helper for Leaflet: Zoom home.
 */
(function ($) {
  'use strict';
  Drupal.behaviors.LeafletZoomHome = {
    attach: function (context, settings) {
      $(document).on('leaflet.map', function (event, map, lMap) {
        // Add Zoom home to the current map.
        var zoomHome = L.Control.zoomHome();
        zoomHome.addTo(lMap);
      });
    }
  };
}(jQuery));
