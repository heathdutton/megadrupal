(function ($) {
  Drupal.behaviors.ColorizedMap = {
    attach: function (context, settings) {
      var blocks = settings['colorized_gmap']['blocks'];
      var item;                               
      for (item in blocks) {
        var block = blocks[item];
        init(blocks[item]);
      }

      function init(block) {
        if (block['style'] == 'undefined') {
          var latitude = 48.853358;
          var longitude = 2.348903;
          var markertitle = 'Destination';
          var mapOptions = {
            zoom: 15,
            center: new google.maps.LatLng(latitude, longitude)
          };
        }
        else {
          var markericon = null;
          if (block['additional_settings']['marker_settings']['marker']['url']) {
            markericon = block['additional_settings']['marker_settings']['marker']['url'];
          }
          var markertitle = block['additional_settings']['marker_settings']['markertitle'];
          var latitude = block['latitude'];
          var longitude = block['longitude'];
          var mapstyle = block['style'];
          var zoomControlSize = block['additional_settings']['zoom_control_settings']['zoomControlSize'];
          var zoomControlPosition = block['additional_settings']['zoom_control_settings']['zoomControlPosition'];
          var mindragwidth = block['additional_settings']['controls']['min_drag_width'];
          var mapOptions = {
            zoom: parseInt(block['additional_settings']['zoom_control_settings']['zoom']),
            center: new google.maps.LatLng(latitude, longitude),
            styles: mapstyle,
            scrollwheel: block['additional_settings']['zoom_control_settings']['scrollwheel'],
            streetViewControl: block['additional_settings']['controls']['streetViewControl'],
            streetViewControlOptions: { 
              position: google.maps.ControlPosition = block['additional_settings']['controls_position']['streetViewControlPosition']
            },
            mapTypeControl: block['additional_settings']['controls']['mapTypeControl'],
            mapTypeControlOptions: { 
              position: google.maps.ControlPosition = block['additional_settings']['controls_position']['mapTypeControlPosition'],
            },
            zoomControl: block['additional_settings']['zoom_control_settings']['zoomControl'],
            draggable: true,
            panControl: block['additional_settings']['controls']['panControl'],
            panControlOptions: { 
              position: google.maps.ControlPosition = block['additional_settings']['controls_position']['panControlPosition']
            },
            zoomControlOptions: { 
              style: google.maps.ZoomControlStyle = zoomControlSize,
              position: google.maps.ControlPosition = zoomControlPosition
            }
          };
          if ($( document ).width() < mindragwidth && mindragwidth != 0) {
            mapOptions.draggable = false;
          }
        }
        var mapId = 'colorized-gmap-' + block['delta'];
        var mapElement = document.getElementById(mapId);
        var map = new google.maps.Map(mapElement, mapOptions);
        marker = new google.maps.Marker({
          position: new google.maps.LatLng(latitude, longitude),
          map: map,
          title: markertitle,
          icon: markericon,
        });
      }  
    }
  };
})(jQuery);