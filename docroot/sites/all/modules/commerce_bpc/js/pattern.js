/**
 * Attach the machine-readable name form element behavior.
 */
(function($) {

  Drupal.behaviors.commerce_bpc = {
    /**
     * Attaches the behavior.
     *
     * @param settings.commerce_bpc
     *   A list of elements to process, keyed by the HTML ID of the form 
     *   element containing the fragment value to be filled in for the token. 
     *   Each element is an object defining the following properties:
     *   - target: The HTML ID of the element containing the example
     *   - target_wrapper : The HTML ID of the element that should be 
     *     shown and hidden, depending on whether the source field has data
     *   - token: A string specifying what should be replaced in pattern.
     *   - pattern: A string containing token.
     */
    attach: function (context, settings) {
      $.each(settings.commerce_bpc, function (source_id, options) {
        $(options.target_wrapper).hide();
        $(source_id, context).bind('change keyup', function() {
          var target = $(options.target, context);
          var target_wrapper = $(options.target_wrapper, context);
          if ($(this).is(':disabled') || $(this).val() == '') {
            target_wrapper.hide();
          }
          else {
            target_wrapper.show();
            target.text(options.pattern.replace(options.token, $(this).val()));
          }
        });
      });
    }
  };
})(jQuery);
