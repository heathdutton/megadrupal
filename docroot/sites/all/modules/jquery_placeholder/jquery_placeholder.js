(function($){

  /**
   * Apply jQuery placeholder plugin to elements whose selectors have been
   * passed down to us.
   */
  Drupal.behaviors.jquery_placeholder = {

    attach: function(context, settings) {
      if (settings.hasOwnProperty('jquery_placeholder')) {
        for (var key in settings.jquery_placeholder) {
          if (settings.jquery_placeholder.hasOwnProperty(key)) {
            var selector = settings.jquery_placeholder[key].selector;

            var $el = $(context).find(selector);
            if ($el.length) {
              // Support $el *being* the input element and *containing* the
              // input element too.
              var placeholder_selector = 'input, textarea';
              $el.find(placeholder_selector)
                .add($el.filter(placeholder_selector))
                .once('jquery_placeholder')
                .placeholder();
            }
          }
        }
      }
    }
  }

})(jQuery);
