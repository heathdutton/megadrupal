(function ($) {
  Drupal.behaviors.simple_subscription = {
      attach: function(context, settings) {
        
        /*
         * If the field is empty, this adds a class to the form and the predefined text to the e-mail
         * input field
         */
        $('#simple-subscription-form .edit-mail', context).blur(function() {
          var val = $(this).val();
          if (!val) {
            $('#simple-subscription-form').addClass('simple-subscription-empty');
            $(this).val(Drupal.settings.simple_subscription.input_content);
          } 
          else if (val == Drupal.settings.simple_subscription.input_content) {
            $('#simple-subscription-form').addClass('simple-subscription-empty');
          };    
        }).focus(function() {
          if ($('#simple-subscription-form').hasClass('simple-subscription-empty')) {
            $(this).val("");
            $('#simple-subscription-form').removeClass('simple-subscription-empty');
          }
        }).blur();

        
        /*
         * Removes the default text in the input field, when the user submits the form without
         * filling it first.
         */
        $('#simple-subscription-form', context).submit(function() {
          if ($(this).hasClass('simple-subscription-empty')) {
            $('#simple-subscription-form').removeClass('simple-subscription-empty');
            $('#simple-subscription-form .edit-mail').val("");
          }
        });    
        
      }
    };  
}(jQuery));

