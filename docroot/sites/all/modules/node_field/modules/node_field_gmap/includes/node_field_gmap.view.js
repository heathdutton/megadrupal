(function($) {
  // Display location on the map.
  Drupal.behaviors.nodeFieldGmapView = {
    attach: function(context, settings) {
      var mapsData = settings.nodeFieldGmapView;
      if (typeof mapsData === 'undefined' || typeof google === 'undefined' || typeof Drupal.Views !== 'undefined') {
        return false;
      }
      $.each(mapsData, function(index, mapData) {
        if ($('#' + mapData.id).hasClass('init')) {
          return;
        }
        var latLon = new google.maps.LatLng(mapData.lat, mapData.lon);
        var mapOptions = {
          zoom: mapData.zoom,
          center: latLon,
          mapTypeId: google.maps.MapTypeId.ROADMAP
        };
        var map = new google.maps.Map(document.getElementById(mapData.id), mapOptions);
        // We need initialize it once.
        $('#' + mapData.id).addClass('init');
        // Set marker with description for saved location.
        if (mapData.marker) {
          var marker = new google.maps.Marker({
            position: latLon,
            map: map
          });
          var infoWindow = new google.maps.InfoWindow({
            content: '<div style="overflow:hidden;">' + mapData.description + '</div>'
          });
          google.maps.event.addListener(marker, 'click', function() {
            infoWindow.open(map, marker);
          });
        }
      });
    }
  }
}(jQuery));
