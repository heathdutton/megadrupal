(function($) {
  // Add location to the map.
  Drupal.behaviors.nodeFieldGmapEdit = {
    attach: function(context, settings) {
      var mapsData = settings.nodeFieldGmapEdit;
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
        // Store maps and markers.
        if (typeof settings.nodeFieldGmap === 'undefined') {
          settings.nodeFieldGmap = {markers: [], maps: []};
        }
        settings.nodeFieldGmap.maps[mapData.id] = map;
        // Search box.
        $('body').prepend('<input style="margin-top:7px;" class="nodefield-gmap-search" id="' + mapData.id + '-search"></input>');
        var input = document.getElementById(mapData.id + '-search');
        map.controls[google.maps.ControlPosition.TOP_LEFT].push(input);
        var searchBox = new google.maps.places.SearchBox(input);
        // Go to the first found location.
        google.maps.event.addListener(searchBox, 'places_changed', function() {
          var places = searchBox.getPlaces();
          if (typeof settings.nodeFieldGmap.markers[mapData.id] === 'object') {
            settings.nodeFieldGmap.markers[mapData.id].setPosition(places[0].geometry.location);
          }
          else {
            var marker = new google.maps.Marker({
              position: places[0].geometry.location,
              map: map
            });
            Drupal.settings.nodeFieldGmap.markers[mapData.id] = marker;
          }
          $(mapData.latId).val(places[0].geometry.location.lat());
          $(mapData.lonId).val(places[0].geometry.location.lng());
          map.setCenter(places[0].geometry.location);
          map.setZoom(mapData.zoom);
        });
        // Set marker for saved location.
        if (mapData.marker) {
          var marker = new google.maps.Marker({
            position: latLon,
            map: map
          });
          Drupal.settings.nodeFieldGmap.markers[mapData.id] = marker;
        }
        // Click map event.
        google.maps.event.addListener(map, 'click', function(e) {
          if (typeof settings.nodeFieldGmap.markers[mapData.id] === 'object') {
            settings.nodeFieldGmap.markers[mapData.id].setPosition(e.latLng);
          }
          else {
            var marker = new google.maps.Marker({
              position: e.latLng,
              map: map
            });
            Drupal.settings.nodeFieldGmap.markers[mapData.id] = marker;
          }
          $(mapData.latId).val(e.latLng.lat());
          $(mapData.lonId).val(e.latLng.lng());
        });
      });
    }
  }
}(jQuery));
