/**
 * @file
 * Functions to configure Pirobox settings in Views module context.
 *
 * Handle form UI in context with the usage of the textfield
 * 'Custom gallery'. Mainly the error management:
 *   - if textfield empty on submit
 *   - display required information on the field
 */

(function ($) {
  Drupal.behaviors.viewsFormatterPirobox = {
    attach: function(context, settings) {
      if ($('form#views-ui-config-item-form').length) {
        // Actions on 'Gallery (image grouping)' select.
        $('.pirobox-gallery-grouping').change(function() {
          // Handle error message display.
          if ($('.pirobox-gallery-grouping').val() != 'custom') {
            $('#pirobox-gallery-custom-error').hide();
            $('.pirobox-gallery-custom').removeClass('error');
          }
        });
        // Actions on 'Content image style' select.
        $('.pirobox-entity-style').change(function() {
          if ($('.pirobox-entity-style').val() == 'hide') {
            $('#pirobox-gallery-custom-error').hide();
            $('.pirobox-gallery-custom').removeClass('error');
          }
          // Handle 'Custom' enabled.
          if ($('.pirobox-entity-style').val() != 'hide') {
            if ($('.pirobox-gallery-grouping').val() == 'custom') {
              $('.pirobox-gallery-custom').addClass('required');
              // Make required information visible.
              var itemL = $('.pirobox-gallery-custom').prev('label');
              if ($(itemL = ' .form-required').length == 0) {
                $('.pirobox-gallery-custom').prev('label').append('<span class="form-required" title="' + Drupal.t("This field is required.") + '">*</span>');
              }
            }
            // Make required information invisible.
            else {
              $('.pirobox-gallery-custom').parent().hide();
            }
          }
        });
        // Actions on form submit.
        // Display error message if empty the 'Custom gallery' field.
        $('#edit-submit').click(function() {
          if ($('.pirobox-entity-style').val() != 'hide' && $('.pirobox-gallery-grouping').val() == 'custom' && $('.pirobox-gallery-custom').val() == '') {
            $('.pirobox-gallery-custom').addClass('error');
            $('#pirobox-gallery-custom-error').show();
            return false;
          }
        });
      }
    }
  };
})(jQuery);
