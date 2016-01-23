
(function ($) {

  Drupal.behaviors.filter_harmonizer = {
    attach: function(context, settings) {
      if (settings.filter_harmonizer_url.length > 0) {
        window.history.pushState(null, "Harmonized filter page " + settings.filter_harmonizer_url, settings.filter_harmonizer_url);
      }
    }
  };

})(jQuery);
