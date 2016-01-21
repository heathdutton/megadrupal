(function($) {
  Drupal.behaviors.better_form = {
    attach: function(context, settings) {
      // Check checkboxes fields
      $('form').once('better-form', function() {
        var form_submit = $(this).find('.form-submit');
        form_submit.click(function() {
          var clicked_submit = $(this);
          var required_checkboxes = clicked_submit.closest('form').find('div.required-form-field');
          required_checkboxes.each(function() {
            var current_required_fields = $(this);
            var required_field = current_required_fields.find('input.required-form-field');
            var first_required_field = required_field.eq(0);
            if (current_required_fields.find('input.required-form-field:checked').length === 0) {
              first_required_field.attr('required', 'required');
            }
            else {
              first_required_field.removeAttr('required');
            }
          });
        });
      });
    }    
  };
})(jQuery);
