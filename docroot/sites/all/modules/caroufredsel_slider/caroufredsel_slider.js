(function ($) {

/**
 * Attach the carouFredSel slider to matching elements
 */
Drupal.behaviors.carouFredSel_slider = {
  attach: function (context, settings) {
    $.each(settings.caroufredsel_slider, function(selector, instance){
      $('#' + selector + ' > ul', context).carouFredSel(instance.config, instance.plugin);
    });
  }
};

})(jQuery);
