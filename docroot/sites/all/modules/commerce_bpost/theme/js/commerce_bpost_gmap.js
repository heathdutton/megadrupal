/**
 * @file
 * Script file to handle the display of a google map.
 */

/**
 * 
 * @param {object} $
 * @param {object} google
 * @param {object} Drupal
 * @param {object} info
 * @returns {undefined}
 */
(function ($) {

  /**
   * @type google.maps.Map
   */
  var map;
  var latlngbounds;
  var markers = {};
  var infoWindows = {};
  var event_listeners = {};

  Drupal.behaviors.commerceBpost = {
    attach: function (context, settings) {
      $('#' + settings.commerce_bpost_map.id).once(function () {
        $.fn.commerceBpostInitialize(context, settings);
      });
    }
  };

  /**
   * Initialize map.
   */
  $.fn.commerceBpostInitialize = function (context, settings) {

    // Create map
    var mapOptions = {
      center: new google.maps.LatLng(-34.397, 150.644),
      //zoom:settings.commerce_bpost_map.zoom,
      streetViewControl: settings.commerce_bpost_map.street_view_control,
      mapTypeControl: settings.commerce_bpost_map.map_type_control,
      mapTypeId: google.maps.MapTypeId.ROADMAP
    };
    map = new google.maps.Map(document.getElementById(settings.commerce_bpost_map.id), mapOptions);

    // Use by auto-zoom function
    latlngbounds = new google.maps.LatLngBounds();

    // Center map
    geocoder = new google.maps.Geocoder();

    $.fn.commerceBpostSetHome(settings, latlngbounds);

    // Create markers
    var markersData = $.parseJSON(settings.commerce_bpost_map.markers);
    for (var i in markersData) {
      var markerData = markersData[i];
      createMarker(markerData);
    }

    pointsListEvents();

    // Listen for zoom changes
    google.maps.event.addListener(map, 'zoom_changed', function (mEvent) {
      var zoom = map.getZoom();
      $('input[name="commerce_shipping[service_details][bpack_map][filters][zoom]"]').val(zoom);
    });

    // Add drag events for map
    google.maps.event.addListener(map, 'dragstart', function (mEvent) {
    });
    //When the drag map event stops, get the center latlang of the map to fetch all post points around the location.
    google.maps.event.addListener(map, 'dragend', function (mEvent) {
      var latlng = map.getCenter();
      // Set loading overlay.
      geocoder.geocode({
        'latLng': latlng
      }, function (results, status) {
        if (status == google.maps.GeocoderStatus.OK) {
          if (results[1]) {
            var zoom = map.getZoom();
            // Clear search fields.
            $('#commerce-shipping-service-details .commerce-bpost-filters input.form-text').val('');
            $('#commerce-shipping-service-details .commerce-bpost-filters input.field-zone').val('');
            var address_components = results[1].address_components;
            // Get the address elements from the Geocoder results.
            for(i in address_components) {
              // Add locality to search fields.
              if(address_components[i].types[0] == 'locality') {
                $('#commerce-shipping-service-details .commerce-bpost-filters input.field-city').val(address_components[i].long_name);
              }
              // Add postal code to search fields.
              if(address_components[i].types[0] == 'postal_code') {
                $('#commerce-shipping-service-details .commerce-bpost-filters input.field-postal-code').val(address_components[i].long_name);
              }
              // Add zone to search fields.
              if(address_components[i].types[0] == 'administrative_area_level_1' && $('#commerce-shipping-service-details .commerce-bpost-filters input.field-zone').val() == '') {
                  $('#commerce-shipping-service-details .commerce-bpost-filters input.field-zone').val(address_components[i].long_name);
              }
              if(address_components[i].types[0] == 'administrative_area_level_2') {
                $('#commerce-shipping-service-details .commerce-bpost-filters input.field-zone').val(address_components[i].long_name);
              }
            }
            // Submit the form only if a locality was return.
            if ($('#commerce-shipping-service-details .commerce-bpost-filters input.field-city').val()) {
              $('#commerce-shipping-service-details .bpost-shipping-services').addClass('bpost-loading');
              // Add zoom to search fields.
              $('input[name="commerce_shipping[service_details][bpack_map][filters][zoom]"]').val(zoom);
              // Submit filters to refresh map.
              $('#commerce-shipping-service-details input.form-submit').click();
            }
          } // End if geocoder failed.
        } else {
        }
      });
    });

    //End of drag events
  };

/**
 * Center map on address.
 * 
 * @param {object} settings
 * @param {object} bounds
 */
  $.fn.commerceBpostSetHome = function (settings, bounds) {
    geocoder.geocode({'address': settings.commerce_bpost_map.address}, function (results, status) {
      if (status == google.maps.GeocoderStatus.OK) {
        var set_center = false;
        // Be sure result is Belgium country.
        for (var address_component_it in results[0].address_components) {
          var address_component = results[0].address_components[address_component_it];
          if (address_component.types[0] == 'country' && address_component.short_name == 'BE') {
            set_center = true;
          }
        }
        if (set_center) {
          map.setCenter(results[0].geometry.location);
          var marker = new google.maps.Marker({
            position: results[0].geometry.location,
            map: map,
            icon: settings.commerce_bpost_map.icons.domicile
          });
          bounds.extend(results[0].geometry.location);
        }
        // Get the previous zoom value, if defined
        var zoom = parseInt($('input[name="commerce_shipping[service_details][bpack_map][filters][zoom]"]').val());
        if (zoom > 0) {
          map.setZoom(zoom);
        }
        else if (settings.commerce_bpost_map.autozoom) {
          map.fitBounds(bounds);
        }
      }
    });
  };

  function createMarker(markerData) {
    var markerLatLng = new google.maps.LatLng(markerData.coordGeolocalisationLatitude, markerData.coordGeolocalisationLongitude);
    var marker = new google.maps.Marker({
      position: markerLatLng,
      map: map,
      id: markerData.identifiant,
      content_map: markerData.content_map,
      content_top: markerData.content_top,
      icon: markerData.icon,
      type: markerData.type
    });
    latlngbounds.extend(markerLatLng);

    // Store the marker so we can access it later.
    markers[markerData.identifiant] = marker;

    // Add InfoWindow for this marker and store it so we can access it later.
    infoWindows[marker.id] = attachInfoWindow(marker, markerData.infoWindow);

    return marker;
  }

  function removeMarker(marker) {
    // Remove listeners for existing markers.
    google.maps.event.removeListener(event_listeners[marker[5]]);
    delete infoWindows[marker.id];
    delete markers[marker.id];
    if (marker !== undefined) {
      marker.setMap(null);
    }
  }

  function removeMarkers() {
    for (var marker_id in markers) {
      var marker = markers[marker_id];
      removeMarker(marker);
    }
    $('#points-wrapper').empty();
    latlngbounds = new google.maps.LatLngBounds();
  }

  function closeAllInfoWindow() {
    for (var i in infoWindows) {
      infoWindows[i].close();
    }
  }

  function pointsListEvents() {
    // Click point in the list
    $('.points-list input[type=radio]').once('points-list-event', function () {
      $(this).click(function () {
        var point = this.value.split("|");
        closeAllInfoWindow();
        infoWindows[point[1]].open(markers[point[1]].get('map'), markers[point[1]]);

        // Clear selected parent labels.
        $('.points-list .form-item.selected').removeClass('selected');
        // Get radio buttons list.
        $(this).parent().addClass('selected');
      });
    });
  }

  // Click point on the map
  function attachInfoWindow(marker, message) {
    var infowindow = new google.maps.InfoWindow({
      content: message
    });
    var clickevent = google.maps.event.addListener(marker, 'click', function () {
      closeAllInfoWindow();
      infowindow.open(marker.get('map'), marker);
      // Clear selected parent labels.
      $('.points-list .form-item.selected').removeClass('selected');
      // Get radio buttons list.
      $('.points-list input[type=radio]').each(
              function (key, domElement) {
                if (String(domElement.value) === markers[marker.id].type.concat('|', marker.id)) {
                  // Add class to parent.
                  domElement.parentNode.classList.add('selected');
                  // Check the found radio box.
                  domElement.checked = true;
                }
              }
      );
      // Point to the selected map point.
      var elementSelected = $('.points-list .form-item.selected');
      var container = $('#points-wrapper');
      container.scrollTop(
              elementSelected.offset().top - container.offset().top + container.scrollTop()
              );
    });

    event_listeners[marker[5]] = clickevent;
    return infowindow;
  }

  // Highlight and select point in points list.
  function selectPoint(element) {
    var input = $('#' + $(element).attr('id') + ' input.select-point');
    var marker_id = input.val();
    var marker = markers[marker_id];
    $('.point-wrapper div.selected').removeClass('selected');
    $(element).addClass('selected');
    input.attr('checked', true);
    $('.point-wrapper').html(marker.content_top);
    info.setContent(marker.content_map);
    info.open(map, marker);
  }
})(jQuery);
