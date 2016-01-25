(function($) {

/**
* Behavior for the jRumble Drupal module.
*/
Drupal.behaviors.jrumble = {
  attach: function(context, settings) {
    $.each(settings.jrumble || {}, function(selector, options) {
      // Initialize jRumble on the element.
      $(selector).jrumble(options).hover(function() {
        $(this).trigger('startRumble');
      }, function() {
        $(this).trigger('stopRumble');
      });
    });
  }
};

})(jQuery);
