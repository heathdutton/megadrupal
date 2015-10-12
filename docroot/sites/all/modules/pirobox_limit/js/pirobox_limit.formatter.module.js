/**
 * @file
 * Required functions to configure Pirobox Limit formatter settings.
 *
 * Realize and handle any multiple form element states.
 */

(function ($) {
  Drupal.behaviors.formatterPiroboxLimit = {
    attach: function(context, settings) {
      // Visibility handling.
      if ($('.pirobox-gallery-grouping').length) {
        // Handle 'Gallery limitation' and 'Gallery limitation bypass'.
        if ($('.pirobox-gallery-grouping').val() == 'none') {
          $('.pirobox-gallery-limit').parent().hide();
          $('.pirobox-gallery-limit-bypass').parent().hide();
        }
      }
      // Visibility handling.
      if ($('.pirobox-gallery-limit').length) {
        // Handle 'Gallery limitation bypass'.
        if ($('.pirobox-gallery-limit').val() == 0) {
          $('.pirobox-gallery-limit-bypass').parent().hide();
        }
      }
      // Actions on 'Content image style' select.
      $('.pirobox-entity-style').change(function() {
        // Handle 'Gallery limitation'.
        if ($('.pirobox-gallery-grouping').val() == 'none') {
          $('.pirobox-gallery-limit').parent().hide();
          $('.pirobox-gallery-limit').val('0');
        }
        // Handle 'Gallery limitation' and 'Gallery limitation bypass'.
        if ($('.pirobox-entity-style').val() == 'hide') {
          $('.pirobox-gallery-limit').parent().hide();
          $('.pirobox-gallery-limit').val('0');
          $('.pirobox-gallery-limit-bypass').parent().hide();
          $('.pirobox-gallery-limit-bypass').val('0');
        }
        // Handle 'Gallery limitation'.
        if ($('.pirobox-entity-style').val() == 'link') {
          if ($('.pirobox-gallery-grouping').val() == 'none') {
            $('.pirobox-gallery-limit').parent().hide();
          }
          else {
            $('.pirobox-gallery-limit').parent().show();
          }
        }
        // Handle 'Gallery limitation' and 'Gallery limitation bypass'.
        if ($('.pirobox-entity-style').val() != 'hide' && $('.pirobox-entity-style').val() != 'link') {
          if ($('.pirobox-gallery-grouping').val() == 'none') {
            $('.pirobox-gallery-limit').parent().hide();
          }
          else {
            $('.pirobox-gallery-limit').parent().show();
          }
          if ($('.pirobox-gallery-limit').val() > 0) {
            $('.pirobox-gallery-limit-bypass').parent().show();
          }
        }
      });
      // Actions on 'Gallery (image grouping)' select.
      $('.pirobox-gallery-grouping').change(function() {
        // Handle 'Gallery limitation' and 'Gallery limitation bypass'.
        if ($('.pirobox-gallery-grouping').val() == 'none') {
          $('.pirobox-gallery-limit').parent().hide();
          $('.pirobox-gallery-limit').val('0');
          $('.pirobox-gallery-limit-bypass').parent().hide();
          $('.pirobox-gallery-limit-bypass').val('0');
        }
        // Handle 'Gallery limitation' and 'Gallery limitation bypass'.
        if ($('.pirobox-gallery-grouping').val() != 'none') {
          $('.pirobox-gallery-limit').parent().show();
          if ($('.pirobox-gallery-limit-bypass').val() > 0) {
            $('.pirobox-gallery-limit-bypass').parent().show();
          }
        }
      });
      // Actions on 'Gallery limitation' select.
      //
      // 'Gallery limitation' and 'Gallery image random'
      // at the same time is not possible.
      $('.pirobox-gallery-limit').change(function() {
        if ($('.pirobox-gallery-limit').val() == 0) {
          // Handle 'Gallery limitation bypass'.
          $('.pirobox-gallery-limit-bypass').parent().hide();
          $('.pirobox-gallery-limit-bypass').val('0');
          // Handle 'Gallery image random'.
          $('#pirobox-limit-random-warning').hide();
          // Enable form buttons.
          $('input[type=submit]').removeAttr('disabled');
        }
        if ($('.pirobox-gallery-limit').val() != 0) {
          // Handle 'Gallery limitation bypass'.
          $('.pirobox-gallery-limit-bypass').parent().show();
          // Check and handle 'Gallery image random '.
          if ($('.pirobox-gallery-random').attr('checked') == true) {
            // Display message.
            $('#pirobox-limit-random-warning').show();
            // Disable form buttons.
            $('input[type=submit]').attr('disabled', 'disabled');
          }
          else {
            // Hide message.
            $('#pirobox-limit-random-warning').hide();
            // Enable form buttons.
            $('input[type=submit]').removeAttr('disabled');
          }
        }
      });
      // Actions on 'Gallery image random' checkbox.
      // 'Gallery limitation' and 'Gallery image random'
      // at the same time is not possible.
      $('.pirobox-gallery-random').change(function() {
        if ($('.pirobox-gallery-limit').val() != 0 && $('.pirobox-gallery-random').attr('checked') == true) {
          // Display message.
          $('#pirobox-limit-random-warning').show();
          // Disable form buttons.
          $('input[type=submit]').attr('disabled', 'disabled');
        }
        if ($('.pirobox-gallery-limit').val() == 0 || $('.pirobox-gallery-random').attr('checked') == false) {
          // Hide message.
          $('#pirobox-limit-random-warning').hide();
          // Enable form buttons.
          $('input[type=submit]').removeAttr('disabled');
        }
      });
    }
  };
})(jQuery);
