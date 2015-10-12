/**
 * @file
 * Script file to handle the display of a google map.
 */

(function ($) {

  Drupal.behaviors.commerceSocolissimoFlexibility = {
    attach: function (context, settings) {
      $('#' + settings.commerce_socolissimo_map.id).once(function () {
        $.fn.commerceSocolissimoFlexibilityInitialize(context, settings);
      });
    }
  }

  /**
   * Initialize map.
   */
  $.fn.commerceSocolissimoFlexibilityInitialize = function (context, settings) {
    // Create map
    var mapOptions = {
      zoom: settings.commerce_socolissimo_map.zoom,
      streetViewControl: settings.commerce_socolissimo_map.street_view_control,
      mapTypeControl: settings.commerce_socolissimo_map.map_type_control,
      mapTypeId: google.maps.MapTypeId.ROADMAP
    }
    map = new google.maps.Map(document.getElementById(settings.commerce_socolissimo_map.id), mapOptions);
    // Use by auto-zoom function
    var latlngbounds = new google.maps.LatLngBounds();

    // Center map
    geocoder = new google.maps.Geocoder();
    $.fn.commerceSocolissimoFlexibilitySetHome(settings, latlngbounds);

    // Tooltip window
    if (typeof InfoBox == 'function') {
      var info = new InfoBox({
        content: "loading...",
        maxWidth: 0,
        pixelOffset: new google.maps.Size(-110, -68),
        alignBottom: true,
        zIndex: null,
        closeBoxMargin: "15px",
        closeBoxURL: "http://www.google.com/intl/en_us/mapfiles/close.gif",
        isHidden: false,
        pane: "floatPane",
        enableEventPropagation: false
      });
    }
    else {
      var info = new google.maps.InfoWindow({
        content: "loading...",
        maxWidth: 0,
        pixelOffset: new google.maps.Size(-110, -68),
        zIndex: null
      })
    }

    // Create markers
    var markers = {};
    var markersData = $.parseJSON(settings.commerce_socolissimo_map.markers);
    for (var i in markersData) {
      var markerData = markersData[i];
      var markerLatLng = new google.maps.LatLng(markerData.coordGeolocalisationLatitude, markerData.coordGeolocalisationLongitude);
      var marker = new google.maps.Marker({
        position: markerLatLng,
        map: map,
        id: markerData.identifiant,
        content_map: markerData.content_map,
        content_top: markerData.content_top,
        icon: markerData.icon
      });
      latlngbounds.extend(markerLatLng);
      markers[markerData.identifiant] = marker;
      if (markerData.default_value) {
        google.maps.event.addListenerOnce(map, 'tilesloaded', function () {
          $('.commerce-socolissimo-point-wrapper .point_informations.selected').once(function () {
            google.maps.event.trigger(markers[$(this).attr('id')], 'click')
          });
        });
      }

      // Open detailed tooltip when clicking on marker
      google.maps.event.addListener(marker, 'click', function () {
        var elementSelected = $('.commerce-socolissimo-point-wrapper #' + this.id);
        var container = $('.commerce-socolissimo-point-wrapper .content');
        selectPoint(elementSelected);
        container.scrollTop(
          elementSelected.offset().top - container.offset().top + container.scrollTop()
        );
      });

      // Trigger marker click when point is selected in points list
      $('#' + markerData.identifiant).click(function () {
        selectPoint(this);
      });
    }

    // Highlight and select point in points list.
    function selectPoint(element) {
      var input = $('#' + $(element).attr('id') + ' input.select-point');
      var marker_id = input.val();
      var marker = markers[marker_id];
      $('.commerce-socolissimo-point-wrapper div.selected').removeClass('selected');
      $(element).addClass('selected');
      input.attr('checked', true);
      $('.point-wrapper').html(marker.content_top);
      info.setContent(marker.content_map);
      info.open(map, marker);
    }

    // Center map on home
    $('.commerce-socolissimo-map-home').click(function () {
      // Close opened tooltip windows
      if (info) {
        info.close();
      }
      $.fn.commerceSocolissimoFlexibilitySetHome(settings, latlngbounds);
    });
    // Auto-zoom
    if (settings.commerce_socolissimo_map.autozoom) {
      map.fitBounds(latlngbounds);
    }
  }

  /**
   * Center map on address
   */
  $.fn.commerceSocolissimoFlexibilitySetHome = function (settings, bounds) {
    geocoder.geocode({ 'address': settings.commerce_socolissimo_map.address}, function (results, status) {
      if (status == google.maps.GeocoderStatus.OK) {
        map.setCenter(results[0].geometry.location);
        var marker = new google.maps.Marker({
          position: results[0].geometry.location,
          map: map,
          icon: settings.commerce_socolissimo_map.icons.domicile
        });
        bounds.extend(results[0].geometry.location);
        if (settings.commerce_socolissimo_map.autozoom) {
          map.fitBounds(bounds);
        }
      }
      else {
        // Use for debug only
        //alert('Geocode was not successful for the following reason: ' + status);
      }
    });
  }

})(jQuery);
