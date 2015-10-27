(function ($) {

// Stores current opened infoWindow.
var currentInfoWindow = null;

/**
 * Create google map.
 *
 * @param object map
 *   Object of map options.
 * @return object
 *   On success returns gmap object.
 */
function gmap3ToolsCreateMap(map) {
  if (map.mapId === null) {
    alert(Drupal.t('gmap3_tools error: Map id element is not defined.'));
    return null;
  }

  // Create map.
  var mapOptions = map.mapOptions;
  mapOptions.center = new google.maps.LatLng(mapOptions.centerX, mapOptions.centerY);
  var gmap = new google.maps.Map(document.getElementById(map.mapId), mapOptions);
  
  // Store gmap in map element so it can be accessed later from js if needed.
  $('#' + map.mapId).data('gmap', gmap);

  // Try HTML5 geolocation.
  if (map.gmap3ToolsOptions.geolocation && navigator.geolocation) {
    navigator.geolocation.getCurrentPosition(function(position) {
      var pos = new google.maps.LatLng(position.coords.latitude, position.coords.longitude);
      gmap.setCenter(pos);
      gmap3ToolsCreateMarkers(map, gmap);
    }, function() {
      // Geolocation service failed.
      gmap3ToolsCreateMarkers(map, gmap);
    });
  }
  else {
    // Do not use geolocation or browser do not support geolocation.
    gmap3ToolsCreateMarkers(map, gmap);
  }

  return gmap;
}

/**
 * Create markers.
 *
 * @param {object} map
 *   gmap3 tools object.
 * @param {Map} gmap
 *   Google Map object.
 */
function gmap3ToolsCreateMarkers(map, gmap) {
  // Array for storing all markers that are on this map.
  var gmapMarkers = [];

  // Create markers for this map.
  var markersNum = 0;
  $.each(map.markers, function (i, markerData) {

    var markerLat = markerData.lat;
    var markerLng = markerData.lng;

    // Is marker relative from map center?
    if (markerData.markerOptions.relative) {
      var pos = gmap.getCenter();
      markerLat = pos.lat() + markerLat;
      markerLng = pos.lng() + markerLng;
    }

    var marker = new google.maps.Marker({
      position: new google.maps.LatLng(markerLat, markerLng),
      map: gmap
    });

    // Marker options.
    var markerOptions = $.extend({}, map.markerOptions, markerData.markerOptions);
    marker.setOptions(markerOptions);

    // Title of marker.
    if (typeof markerData.title !== 'undefined') {
      marker.setTitle(markerData.title);
    }

    // If marker has content then create info window for it.
    if (typeof markerData.content !== 'undefined') {
      google.maps.event.addListener(marker, 'click', function(e) {
        if (map.gmap3ToolsOptions.closeCurrentInfoWindow &&  currentInfoWindow != null) {
          currentInfoWindow.close();
        }
        var infoWindowOptions = map.infoWindowOptions;
        infoWindowOptions.position = marker.getPosition();
        infoWindowOptions.content = markerData.content;
        infoWindowOptions.map = gmap;
        currentInfoWindow = new google.maps.InfoWindow(infoWindowOptions);
      });
    }

    // Draggable markers with lat and lng form items support.
    if (markerOptions.draggable && (markerOptions.dragLatElement || markerOptions.dragLngElement)) {
      var $latItem = $(markerOptions.dragLatElement);
      var $lngItem = $(markerOptions.dragLngElement);
      google.maps.event.addListener(marker, 'drag', function() {
        $latItem.val(marker.getPosition().lat());
        $lngItem.val(marker.getPosition().lng());
      });
    }

    ++markersNum;
    gmapMarkers.push(marker);
  });

  if (markersNum) {
    // If we are centering markers on map we should move map center near makers.
    // We are doing this so first map center (on first display) will be near
    // map center when all markers are displayed - we will avoid map move
    // when map displays markers.
    // @todo - this can be more smarter - first get exact center from markers
    // and then apply it.
    if (map.gmap3ToolsOptions.defaultMarkersPosition !== 'default') {
      map.mapOptions.center = new google.maps.LatLng(map.markers[0].lat, map.markers[0].lng);
    }

    // Default markers position on map.
    if (map.gmap3ToolsOptions.defaultMarkersPosition === 'center') {
      gmap3ToolsCenterMarkers(gmap, map.markers, markersNum);
    }
    else if (map.gmap3ToolsOptions.defaultMarkersPosition === 'center zoom') {
      var bounds = new google.maps.LatLngBounds();
      for (var i = 0; i < markersNum; i++) {
        var marker = map.markers[i];
        bounds.extend(new google.maps.LatLng(marker.lat, marker.lng));
      }
      gmap.fitBounds(bounds);
    }
  }

  // Store markers in map element so it can be accessed later from js if needed.
  $('#' + map.mapId).data('gmapMarkers', gmapMarkers);
}

/**
 * Center markers on map.
 */
function gmap3ToolsCenterMarkers(map, markers, markersNum) {
  var centerLat = 0;
  var centerLng = 0;
  $.each(markers, function (i, markerData) {
    centerLat += parseFloat(markerData.lat);
    centerLng += parseFloat(markerData.lng);
  });
  centerLat /= markersNum;
  centerLng /= markersNum;
  map.setCenter(new google.maps.LatLng(centerLat, centerLng));
}

/**
 * Attach gmap3_tools maps.
 */
Drupal.behaviors.gmap3_tools = {
  attach: function (context, settings) {
    // Create all defined google maps.
    if (Drupal.settings.gmap3_tools === undefined) {
      return;
    }
    $.each(Drupal.settings.gmap3_tools.maps, function(i, map) {
      // @todo - we should really use css selector here and not only element id.
      var $mapElement = $('#' + map.mapId, context);
      if ($mapElement.length === 0 || $mapElement.hasClass('gmap3-tools-processed')) {
        return;
      }
      $mapElement.addClass('gmap3-tools-processed');
      var gmap = gmap3ToolsCreateMap(map);
    });
  }
};

})(jQuery);
