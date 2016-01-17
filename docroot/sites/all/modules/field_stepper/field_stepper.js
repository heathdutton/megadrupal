(function ($) {
  
  Drupal.behaviors.fieldStepper = {
    attach: function(context, settings) {

      $.each(settings.fieldStepper, function (id, field_settings) {
        
        var field = $('#' + id, context);

        // Only creates the widget if it's hasn't been processed yet
        if (!$(field).hasClass('field-stepper')) {

          // Creates the Spinner Widget
          $(field).spinner(field_settings);

          // Has been processed
          $(field).addClass('field-stepper');
        }

      });

    }
  };

})(jQuery);