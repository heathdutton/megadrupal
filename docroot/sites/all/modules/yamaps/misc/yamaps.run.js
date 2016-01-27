/**
 * @file
 * Runs map.
 */

(function($) {
  Drupal.behaviors.yamapsRun = {
    attach: function (context, settings) {
      ymaps.ready(function () {
        function creating_map(mapId, options) {
          $('#' + mapId).once('yamaps', function () {
            // If zoom and center are not set - set it from user's location.
            if (!options.init.center || !options.init.zoom) {
              var location = ymaps.geolocation;
              // Set map center.
              if (!options.init.center) {
                // Set location, defined by ip, if they not defined.
                options.init.center = [location.latitude, location.longitude];
              }
              if (!options.init.zoom) {
                options.init.zoom = location.zoom ? location.zoom : 10;
              }
            }
            // Create new map.
            var map = new $.yaMaps.YamapsMap(mapId, options);
            if (options.controls) {
              // Enable controls.
              map.enableControls();
            }
            if (options.traffic) {
              // Enable traffic.
              map.enableTraffic();
            }
            // Enable plugins.
            map.enableTools();
          });
        }

        function processMaps() {
          if (Drupal.settings.yamaps) {
            for (var mapId in Drupal.settings.yamaps) {
              var options = Drupal.settings.yamaps[mapId];
              creating_map(mapId, options);
            }
          }
        }

        function openMap(selectorOpen, selectorClose) {
          $('div' + selectorOpen).click(function() {
            var mapId = $(this).attr('mapid');
            $('#' + mapId).removeClass('element-invisible');
            $(this).addClass('element-invisible');
            $('div[mapid="' + mapId + '"]' + selectorClose).removeClass('element-invisible');
          });

          $('div' + selectorClose).click(function() {
            var mapId = $(this).attr('mapid');
            $('#' + mapId).addClass('element-invisible');
            $(this).addClass('element-invisible');
            $('div[mapid="' + mapId + '"]' + selectorOpen).removeClass('element-invisible');
          })
        }

        // Initialize layouts.
        $.yaMaps.initLayouts();
        processMaps();
        openMap('.open_yamap_button', '.close_yamap_button');
      })
    }
  }
})(jQuery);
