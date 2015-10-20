/**
 * @file
 * RoyalSlider loader.
 */
(function ($, Drupal) {
  Drupal.behaviors.royalslider = {
    attach: function(context, settings) {
      if (settings.royalslider) {
        if (settings.royalslider.instances) {
          for (var id in settings.royalslider.instances) {
            var $slider = $('#' + id, context),
              optionset_name = settings.royalslider.instances[id].optionset,
              optionset = settings.royalslider.optionsets[optionset_name];

            // Override slider auto-scale dimensions.
            if (settings.royalslider.instances[id].slider_width) {
              optionset.autoScaleSliderWidth = settings.royalslider.instances[id].slider_width;
              $slider.css('width', optionset.autoScaleSliderWidth);
            }
            if (settings.royalslider.instances[id].slider_height) {
              optionset.autoScaleSliderHeight = settings.royalslider.instances[id].slider_height;
              $slider.css('height', optionset.autoScaleSliderHeight );
            }

            $slider.royalSlider(optionset);
          }
        }
      }
    }
  };
})(jQuery, Drupal);
