/**
 * @file
 * Generates map for retrieving the latitude and longitude of a desired location.
 */

(function () {
  "use strict";
  function initialize() {
    var element = document.getElementById('weathercheck_AdminMap');
    if (typeof (element) != 'undefined' && element != null) {
      var latlng = new google.maps.LatLng(Drupal.settings.wc_latlong.wc_lat, Drupal.settings.wc_latlong.wc_long);
      var map = new google.maps.Map(document.getElementById('weathercheck_AdminMap'), {
        center: latlng,
        zoom: 11,
        mapTypeId: google.maps.MapTypeId.ROADMAP
      });
      var marker = new google.maps.Marker({
        position: latlng,
        map: map,
        draggable: true
      });
      google.maps.event.addListener(marker, 'dragend', function (a) {
        document.getElementById('edit-weathercheck-latitude').value = a.latLng.lat().toFixed(4);
        document.getElementById('edit-weathercheck-longitude').value = a.latLng.lng().toFixed(4);
      });
    }
  }

  google.maps.event.addDomListener(window, 'load', initialize);
})();
