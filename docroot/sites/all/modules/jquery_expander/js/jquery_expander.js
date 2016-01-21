(function ($) {

/**
 * jQuery expander.
 */
Drupal.behaviors.jqueryExpander = {
  attach: function (context) {
  
    var expander = Drupal.settings.jqueryExpander;
      // Add the jQuery expander.
      for (var key in expander) {
        $('.field-expander-' + key).expander(expander[key]);
      }
    }
};

})(jQuery);