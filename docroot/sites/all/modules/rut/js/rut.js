/**
 * @file
 * Setting the elements to use jquery plugin rut.
 */

(function ($) {
  Drupal.behaviors.rutField = {
    attach: function(context) {
      $('.rut-field-input.rut-validate-js', context).each(function() {
        var $this = $(this, context);
        var processed = $this.hasClass('rut-processed');
        if (processed == false) {
          var $message = $this.parent('.form-item').children('.error-message-js');
          $this.Rut({
            on_error: function(){
              $this.addClass('error');
              $message.removeClass('element-invisible');
            },
            on_success: function() {
              $this.removeClass('error');
              $message.addClass('element-invisible');
            },
            format_on: 'keyup'
          }).addClass('rut-processed');
          
        }
        
      });
    }
  }
})(jQuery)
