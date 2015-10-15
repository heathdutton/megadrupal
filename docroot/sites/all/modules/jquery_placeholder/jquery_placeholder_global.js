(function($){

  /**
   * Enable on all textfield/textarea inputs.
   */
  Drupal.behaviors.jquery_placeholder_global = {

    attach: function(context, settings) {

      $(context).find('input, textarea')
        .once('jquery_placeholder')
        .placeholder();

    }

  }

})(jQuery);
