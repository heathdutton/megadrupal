/**
 * @file
 * Initiates map(s) for the Styled Google Map module.
 *
 * A single or multiple Styled Google Maps will be initiated.
 * Drupal behaviors are used to make sure ajax called map(s) are correctly loaded.
 */

(function ($) {
  Drupal.behaviors.styled_google_map = {
    attach: function (context) {
      var maps = Drupal.settings.styled_google_map;
      for (i in maps) {
        var current_map = Drupal.settings['id' + maps[i]];
        var map_id = current_map.id;
        if ($('#' + map_id).length) {
          var map_location = current_map.location;
          var map_settings = current_map.settings;
          var bounds = new google.maps.LatLngBounds();
          var map_types = {
            'ROADMAP': google.maps.MapTypeId.ROADMAP,
            'SATELLITE': google.maps.MapTypeId.SATELLITE,
            'HYBRID': google.maps.MapTypeId.HYBRID,
            'TERRAIN': google.maps.MapTypeId.TERRAIN
          }
          var map_style = (map_settings.style.style != '' ? map_settings.style.style : '[]');
          var init_map = {
            zoom: parseInt(map_settings.zoom.default),
            mapTypeId: map_types[map_settings.style.maptype],
            disableDefaultUI: !map_settings.ui,
            maxZoom: parseInt(map_settings.zoom.max),
            minZoom: parseInt(map_settings.zoom.min),
            styles: JSON.parse(map_style),
            mapTypeControl: map_settings.maptypecontrol,
            panControl: map_settings.pancontrol,
            streetViewControl: map_settings.streetviewcontrol,
            zoomControl: map_settings.zoomcontrol,
            scrollwheel: map_settings.scrollwheel,
          }
          var map = new google.maps.Map(document.getElementById(map_id), init_map);
          var infowindow = new google.maps.InfoWindow({content: "holding..."});
          var marker = new google.maps.Marker({
            position: new google.maps.LatLng(map_location.lat , map_location.lon),
            map: map,
            html: map_settings.popup.text,
            icon: map_settings.style.pin
          });
          if (map_settings.popup.text) {
            google.maps.event.addListener(marker, 'click', (function(m){
                return function() {
                    infowindow.setContent(this.html);
                    infowindow.open(m, this);
                };
            }(map)));
          }
          bounds.extend(marker.getPosition());
          map.setCenter(bounds.getCenter());
        }
      }
      // Prevents piling up generated map ids.
      Drupal.settings.styled_google_map = [];
    }
  };
})(jQuery);
