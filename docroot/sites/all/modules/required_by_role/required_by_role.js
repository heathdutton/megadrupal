(function($) {
/**
 * Custom behavior for the module required_by_role
 * States api does not solve the problem.
 */
Drupal.behaviors.requiredByRole = {
  attach: function (context, settings) {

    jQuery(':input[name="instance[required]"]').change(function() {
      if(jQuery(this).is(':checked')) {
        jQuery('.tableselect-required-by-role').find(':checkbox').attr('disabled', 'disabled');
      }
      else {
       jQuery('.tableselect-required-by-role').find(':checkbox').removeAttr('disabled');
      }
    });

    jQuery('.tableselect-required-by-role :checkbox').change(function() {
      if(jQuery(this).is(':checked')) {
        jQuery(':input[name="instance[required]"]').attr('checked', false);
      }
    });

    jQuery(':input[name="instance[required]"]').change();

  }
};

})(jQuery);
