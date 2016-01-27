/**
 * @file
 * This file contain the javascript necessary to display google maps created
 * using the WYSIWYG inline map token builder.
 */

(function ($) {
  Drupal.behaviors.wysiwyg_map_tokens = {
    attach: function (context) {
      var map_collection = Drupal.settings.wysiwyg_map_maps;
      var map = [];
      for (var i = 0; i < map_collection.length; i++) {
        var latlng = new google.maps.LatLng(map_collection[i]['lat'], map_collection[i]['lon']);
        switch(map_collection[i]['map_type']) {
          case 'satellite':
            var mapOptions = {
              zoom: parseInt(map_collection[i]['zoom']),
              center: latlng,
              streetViewControl: false,
              mapTypeId: google.maps.MapTypeId.SATELLITE
            };
            break;

          case 'hybrid':
            var mapOptions = {
              zoom: parseInt(map_collection[i]['zoom']),
              center: latlng,
              streetViewControl: false,
              mapTypeId: google.maps.MapTypeId.HYBRID
            };
            break;

          case 'terrain':
            var mapOptions = {
              zoom: parseInt(map_collection[i]['zoom']),
              center: latlng,
              streetViewControl: false,
              mapTypeId: google.maps.MapTypeId.TERRAIN
            };
            break;

          default:
            var mapOptions = {
              zoom: parseInt(map_collection[i]['zoom']),
              center: latlng,
              streetViewControl: false,
              mapTypeId: google.maps.MapTypeId.ROADMAP
            };
            break;

        }
        // Create the map and drop it into the relavent container.
        var mapContainer = document.getElementById("wysiwyg_map_" + map_collection[i]['container_id']);
        if (mapContainer) {
          map[i] = new google.maps.Map(document.getElementById("wysiwyg_map_" + map_collection[i]['container_id']), mapOptions);
          // Add the marker to the map.
          marker = new google.maps.Marker({
            position: latlng,
            map: map[i]
          });
        }
      }
    }
  };
})(jQuery);
