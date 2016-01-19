/**
 * Defines the TagCanvas and attachs them to the tagadelic block
 */
(function ($) {
  Drupal.behaviors.tagcanvas = {
    attach: function(context, settings) {
      var options = settings.tagcanvas;

      if (options['cssSelector'] && options['canvasId']) {
        var canvas = $('#' + options['canvasId'], context);
        var canvasContainer = $('#' + options['canvasId'] + '-container');

        canvas.attr({
          'width': options['canvasWidth'] == 'auto' ? canvasContainer.width() : options['canvasWidth'],
          'height': options['canvasHeight'] == 'auto' ? canvasContainer.height() : options['canvasHeight']
        });

        if (options['google_webfont_loader_api']) {
          // ensure the web font are loaded before calling tagcanvas
          Drupal.settings.google_webfont_loader_api_setting.active = function() {
            console.log('web font loaded')
            if (!canvas.tagcanvas(options, options['cssSelector'])) {
              // something went wrong, hide the canvas container
              canvas.hide();
            }
          };
        }
        else if (!canvas.tagcanvas(options, options['cssSelector'])) {
          // something went wrong, hide the canvas container
          canvas.hide();
        }
      }
    }
  }
})(jQuery);