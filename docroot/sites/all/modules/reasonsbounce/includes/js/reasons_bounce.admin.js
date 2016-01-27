(function($){
  
  Drupal.behaviors.reasons_bounce = {
    
    attach: function(context) {
      $('fieldset.node-form-responce-bounce', context).drupalSetSummary(function (context) {
        var bounceCheckbox = $('.form-item-bounce-responce-bounce-enabled input', context);
        
        if (bounceCheckbox.is(':checked')) {
          return Drupal.t('Use as form for bounce');
        }

        return Drupal.t('Do not use');
      });

    }
    
  }
  
})(jQuery);