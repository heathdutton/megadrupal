/**
 * @file
 * This file contains all JQuery based functionality for files.
 */
(function ($) {
  "use strict";

  Drupal.behaviors.openlucius_events = {
    attach: function (context, settings) {

      if (context == document) {

        // Bind to ready state.
        $(document).ready(function () {
          var location = $('#map-location');
          if (location.length > 0) {
            $.ajax({
              method: "GET",
              url: "http://maps.google.com/maps/api/geocode/json?address=" + location.val() + "&sensor=false",
              dataType: "json",
              success: function(data) {

                if (data.results.length > 0 && data.results[0].hasOwnProperty('geometry')) {
                  var location = data.results[0].geometry.location;

                  // Create new latlng for marker and position.
                  var myLatlng = new google.maps.LatLng(location.lat, location.lng);

                  // Initialize options.
                  var mapOptions = {
                    zoom: 10,
                    center: myLatlng,
                    disableDefaultUI: true
                  };

                  // Initialize map.
                  var map = new google.maps.Map(document.getElementById('map-canvas'), mapOptions);

                  // Add marker to map.
                  var marker = new google.maps.Marker({
                    position: myLatlng,
                    map: map
                  });
                }
              }
            });
          }
        });
      }
    }
  };
})(jQuery);
