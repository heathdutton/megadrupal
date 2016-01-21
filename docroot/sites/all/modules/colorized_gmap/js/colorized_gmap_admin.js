(function ($) {
  Drupal.behaviors.ColorPickerfields = {
    attach: function (context, settings) {  
      if ($('#gmap-ajax-wrapper tbody .form-type-textfield input.edit_color_input').length) {
        $('#gmap-ajax-wrapper tbody .form-type-textfield input.edit_color_input').ColorPicker({
          onSubmit: function(hsb, hex, rgb, el) {
            $(el).ColorPickerHide();
            $(el).val('#' + hex);
            $(el).trigger('textfield_change');
          },
          onBeforeShow: function () {
            $(this).ColorPickerSetColor(this.value);
          }
        });
      }
    }
  };
  
  Drupal.behaviors.ColorizedMap = {
    attach: function (context, settings) {
      var markertitle = 'Destination';
      if (typeof settings['colorized_gmap'] == 'undefined') {
        var latitude = 48.853358;
        var longitude = 2.348903;
        var mapOptions = {
          zoom: 15,
          center: new google.maps.LatLng(latitude, longitude)
        };
      }
      else {
        var markericon = null;
        if (settings['colorized_gmap']['additional_settings']['marker_settings']['marker']['url']) {
          markericon = settings['colorized_gmap']['additional_settings']['marker_settings']['marker']['url'];
        }
        var markertitle = settings['colorized_gmap']['additional_settings']['marker_settings']['markertitle'];
        var latitude = settings['colorized_gmap']['lat'];
        var longitude = settings['colorized_gmap']['long'];
        var mapstyle = settings['colorized_gmap']['style'];
        var zoomControlSize = settings['colorized_gmap']['additional_settings']['zoom_control_settings']['zoomControlSize'];
        var zoomControlPosition = settings['colorized_gmap']['additional_settings']['zoom_control_settings']['zoomControlPosition'];
        var mapOptions = {
          zoom: parseInt(settings['colorized_gmap']['additional_settings']['zoom_control_settings']['zoom']),
          center: new google.maps.LatLng(latitude, longitude),
          styles: mapstyle,
          scrollwheel: Drupal.settings['colorized_gmap']['additional_settings']['zoom_control_settings']['scrollwheel'],
          streetViewControl: Drupal.settings['colorized_gmap']['additional_settings']['controls']['streetViewControl'],
          streetViewControlOptions: { 
            position: google.maps.ControlPosition = settings['colorized_gmap']['additional_settings']['controls_position']['streetViewControlPosition']
          },
          mapTypeControl: settings['colorized_gmap']['additional_settings']['controls']['mapTypeControl'],
          mapTypeControlOptions: { 
            position: google.maps.ControlPosition = settings['colorized_gmap']['additional_settings']['controls_position']['mapTypeControlPosition'],
          },
          zoomControl: settings['colorized_gmap']['additional_settings']['zoom_control_settings']['zoomControl'],
          draggable: true,
          panControl: settings['colorized_gmap']['additional_settings']['controls']['panControl'],
          panControlOptions: { 
            position: google.maps.ControlPosition = settings['colorized_gmap']['additional_settings']['controls_position']['panControlPosition']
          },
          zoomControlOptions: { 
            style: google.maps.ZoomControlStyle = zoomControlSize,
            position: google.maps.ControlPosition = zoomControlPosition
          }
        };
      }
      var mapElement = document.getElementById('colorized-gmap-content');
      var map = new google.maps.Map(mapElement, mapOptions);
      marker = new google.maps.Marker({
        position: new google.maps.LatLng(latitude, longitude),
        map: map,
        title: markertitle,
        icon: markericon,
     });
    }
  };
})(jQuery);