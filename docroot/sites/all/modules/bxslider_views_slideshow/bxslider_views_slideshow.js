/**
 *  @file
 *  Initiate the BxSlider plugin.
 */

(function ($) {
  Drupal.behaviors.viewsSlideshowBxslider = {
    attach: function (context, settings) {
      for (id in Drupal.settings.viewsSlideshowBxslider) {
        var html_id = id.replace(/_/g, '-');
        $('#' + html_id + ':not(.viewsSlideshowBxslider-processed)', context).addClass('viewsSlideshowBxslider-processed').each(function () {
          // Fire up the gallery.
          var settingsBxSlider = $.extend(
            {},
            Drupal.settings.viewsSlideshowBxslider[id]['general'],
            Drupal.settings.viewsSlideshowBxslider[id]['controlsfieldset'],
            Drupal.settings.viewsSlideshowBxslider[id]['pagerfieldset'],
            Drupal.settings.viewsSlideshowBxslider[id]['autofieldset'],
            Drupal.settings.viewsSlideshowBxslider[id]['carousel']
          );

          for (callback in Drupal.settings.viewsSlideshowBxslider[id]['callback']) {
            Drupal.settings.viewsSlideshowBxslider[id]['callback'][callback]
              = eval("Drupal.settings.viewsSlideshowBxslider[id]['callback'][callback] = (" + Drupal.settings.viewsSlideshowBxslider[id]['callback'][callback] + ")");
          }
          settingsBxSlider = $.extend({}, settingsBxSlider, Drupal.settings.viewsSlideshowBxslider[id]['callback']);
          // Bind to window load, so that images are responsive.
          $(window).bind('load', {
            bxSlider: $(this)
          }, function (event) {
            event.data.bxSlider.bxSlider(settingsBxSlider);
          });
        });
      }
    }
  };
}(jQuery));
