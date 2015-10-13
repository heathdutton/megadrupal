/**
 * @file
 * This file contains the javascript functions used by the WYSIWYG Map
 * token builder WYSIWYG plugin.
 */

/**
 * Declare global variable by which to identify the map for the token builder.
 */
var wysiwyg_map_mapTokenBuilder;

/**
 * This function is called by the WYSIWYG plugin code when the map builder
 * overlay is opened.
 */
function wysiwyg_map_getMap(lat, lon, zoomAmnt, mapType, css_class) {
  // Set default coords for the map centre and marker.
  if (lat == null && lon == null && zoomAmnt == null) {
    var latlng = new google.maps.LatLng(51.0, 0.12);
    zoomAmnt = 9;
  } else {
    var latlng = new google.maps.LatLng(lat, lon);
  }
  switch(mapType) {
    case 'satellite':
      var mapOptions = {
        zoom: zoomAmnt,
        center: latlng,
        streetViewControl: false,
        mapTypeId: google.maps.MapTypeId.SATELLITE
      };
      break;

    case 'hybrid':
      var mapOptions = {
        zoom: zoomAmnt,
        center: latlng,
        streetViewControl: false,
        mapTypeId: google.maps.MapTypeId.HYBRID
      };
      break;

    case 'terrain':
      var mapOptions = {
        zoom: zoomAmnt,
        center: latlng,
        streetViewControl: false,
        mapTypeId: google.maps.MapTypeId.TERRAIN
      };
      break;

    default:
      var mapOptions = {
        zoom: zoomAmnt,
        center: latlng,
        streetViewControl: false,
        mapTypeId: google.maps.MapTypeId.ROADMAP
      };
      break;

  }
  // Create and attached the map.
  wysiwyg_map_mapTokenBuilder = new google.maps.Map(document.getElementById("wysiwyg_map_selector"), mapOptions);
  // Create the marker and drop it on the map.
  marker = new google.maps.Marker({
    position: latlng,
    draggable: true,
    map: wysiwyg_map_mapTokenBuilder
  });
  // Add a click listener to the map.
  google.maps.event.addListener(wysiwyg_map_mapTokenBuilder, "click", function(event) {
    wysiwyg_map_setMarker(event.latLng)
    wysiwyg_map_buildToken();
  });
  // Add an event listener to the map to catch zoom in/zoom out.
  google.maps.event.addListener(wysiwyg_map_mapTokenBuilder, "zoom_changed", function(event) {
    wysiwyg_map_buildToken();
  });
  // Add an event listener to the map to catch map type change.
  google.maps.event.addListener(wysiwyg_map_mapTokenBuilder, "maptypeid_changed", function(event) {
    wysiwyg_map_buildToken();
  });
  // Add listener to detect marker drag-end
  google.maps.event.addListener(marker, 'dragend', function(event) {
    wysiwyg_map_mapTokenBuilder.setCenter(event.latLng);
    wysiwyg_map_buildToken();
  });
};

/**
 * This function is called when the map zoom changes or the user clicks to
 * drop a marker. It takes the necessary values (lat/long/width/height and
 * zoom) and builds a token from them which is placed in the token field.
 */
function wysiwyg_map_buildToken() {
  var $ = jQuery.noConflict();
  $("#edit-map-type").val(wysiwyg_map_mapTokenBuilder.getMapTypeId());
  $("#edit-zoom").val(wysiwyg_map_mapTokenBuilder.getZoom());
	$("#edit-token").val("lat=" + marker.getPosition().lat() + "~lon=" + marker.getPosition().lng() + "~map_width=" + $("#edit-width").val() + "~map_height=" + $("#edit-height").val() + "~zoom=" + wysiwyg_map_mapTokenBuilder.getZoom() + "~map_type=" + wysiwyg_map_mapTokenBuilder.getMapTypeId() + "~css_class=" + $("#edit-css-class").val());
  return false;
}

/**
 * This function takes the lat/long values passed in and centres the map
 * at that point and also drops a marker at that point.
 */
function wysiwyg_map_setMarker(latLng) {
  marker.setPosition(latLng);
  wysiwyg_map_mapTokenBuilder.setCenter(latLng);
}

/**
 * This function takes the value in the center on field and centers the map
 * at that point.
 */
function wysiwyg_map_doCenterPopup() {
  var geocoder = new google.maps.Geocoder();
  var location = document.getElementById("edit-center-on").value;
  geocoder.geocode({ 'address': location}, function (result, status) {
    if (status == 'OK') {
      var latlng = new google.maps.LatLng(result[0].geometry.location.lat(), result[0].geometry.location.lng());
      wysiwyg_map_mapTokenBuilder.panTo(latlng);
      marker = new google.maps.Marker({
        position: latlng,
        map: wysiwyg_map_mapTokenBuilder
      });
      wysiwyg_map_buildToken();
    } else {
      alert('Could not find location.');
    }
    return false;
  });
  return false;
}
