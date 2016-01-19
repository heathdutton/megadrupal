/**
 * @file
 * JS to init slider pro.
 */

(function($) {
  Drupal.behaviors.sliderPro = {
    sliders: [],
    attach: function(context, settings) {
      for (id in settings.sliderPro.instances) {
        // Store sliders so that developers can change options.
        Drupal.behaviors.sliderPro.sliders[id] = settings.sliderPro.instances[id];
        _slider_pro_init(id, Drupal.behaviors.sliderPro.sliders[id], context);
      }
    }
  };

  function _slider_pro_init(id, optionset, context) {
    $('#slider-pro-' + id, context).sliderPro(optionset);
  }

})(jQuery);
