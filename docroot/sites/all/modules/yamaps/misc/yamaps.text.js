/**
 * @file
 *  Contains functions to control yandex formatter for  text fields.
 */

(function ($) {
  Drupal.behaviors.yamapsText = {
    attach: function (context, settings) {
      $('.yamaps-map-text-container', context).once('yamap').each(function () {
        var $this = $(this);
        var address = $this.data('address');
        // Get URL from the data attribute.
        var url = Drupal.settings.yamapsGeocoderUrl + '&geocode=' + address;
        // We should make an ajax request to the yandex geocode server to get
        // coordinates of the current address.
        $.ajax({
          method: 'GET',
          url: url,
          success: function (data, status, jqXHR) {
            // Checking response
            if (typeof data.response.GeoObjectCollection != 'undefined') {
              var collection = data.response.GeoObjectCollection;
              if (typeof collection.featureMember[0] != 'undefined') {
                // We will get only first found coordinates of the current address.
                var geoObject = collection.featureMember[0]['GeoObject'];
                // Location is the string that contains altitude and attitude
                // separated by space.
                var location = geoObject.Point.pos.split(' ');
                // Add options on the unified yamaps object to use common module
                // functions to render map.
                var id = $this.attr('id');
                // Copy settings form predefined object
                Drupal.settings.yamaps[id] = Drupal.settings['yamaps-row'][id];
                // Add coordinates of the address into object.
                var coordinates = [
                  // For some reason attitude and altitude are swapped in the
                  // ajax response. Maybe it is an Yandex Geolocation service bug.
                  location[1],
                  location[0]
                ];

                Drupal.settings.yamaps[id].init.center = coordinates;
                // Set placemarks coordinate
                if (typeof Drupal.settings.yamaps[id].placemarks != 'undefined') {
                  Drupal.settings.yamaps[id].placemarks[0].coords = coordinates;
                }
                // Runs map rendering.
                Drupal.behaviors.yamapsRun.attach();
              }
            }
          }
        });
      });
    }
  }
})(jQuery);
