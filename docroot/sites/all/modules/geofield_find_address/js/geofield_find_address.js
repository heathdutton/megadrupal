/**
 * @file
 * Display google maps and populate geofield.
 */

(function ($) {

  Drupal.behaviors.addGeofieldMap = {
    attach: function (context, settings) {
      var maps = [];
      var geocoder;
      var markers = [];

      $("button.address-gmap-find-address-button", context).once(function(){
        $(this).on('click', this, function (e) {

          var address_parts_array = [];
          var itemId = $(this).val();
          var delta = $(this).attr("data-delta");
          var gmapId = 'map-canvas-' + itemId;
          var addrField = $(this).attr('data-field-name');

          // Initialize map if it doesn't exist.
          if (!maps[gmapId]) {
            initialize(gmapId);
          }

          $("fieldset#" + itemId + " .form-item input[type=text], fieldset#" + itemId + " .form-item select > option:selected", context).each(function () {
            if ($(this).is('option')) {
              address_parts_array.push($(this).text());
            }
            else if ($(this).val()){
              address_parts_array.push($(this).val());
            }
          });

          var location_field_separator = ', ';
          var address_string = address_parts_array.join(location_field_separator);

          if (google.maps.version !== 'undefined') {
            geocoder = new google.maps.Geocoder();
            geocoder.geocode({'address': address_string}, function (results, status) {
              if (status == google.maps.GeocoderStatus.OK) {
                var latlng = results[0].geometry.location;
                if (markers[gmapId]) {
                  markers[gmapId].setPosition(latlng);
                }
                else {
                  markers[gmapId] = new google.maps.Marker({
                    position: latlng,
                    map: maps[gmapId]
                  });
                }
                $("input#" + addrField + "-lat-" + delta + "-geom").attr('value', latlng.lat());
                $("input#" + addrField + "-lon-" + delta + "-geom").attr('value', latlng.lng());
                $('#map-canvas-' + itemId).css('display', 'block');
                google.maps.event.trigger(maps[gmapId], "resize");
                maps[gmapId].setZoom(14);
                maps[gmapId].setCenter(latlng);
              }
              else {
                alert(Drupal.t("Your address was not found."));
                return false;
              }
            });
          }

          return false;
        });
      });

      function initialize(mapId) {
        var mapCanvas;
        var mapOptions = {
          zoom: 8,
          center: new google.maps.LatLng(38.9059544, -77.04192560000001)
        };
        mapCanvas = document.getElementById(mapId);
        maps[mapId] = new google.maps.Map(mapCanvas, mapOptions);
      }
    }
  };
})(jQuery);
