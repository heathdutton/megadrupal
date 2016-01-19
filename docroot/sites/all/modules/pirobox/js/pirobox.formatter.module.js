/**
 * @file
 * Required functions to configure Pirobox formatter settings.
 *
 * Realize multiple states properties.
 */

(function ($) {
  Drupal.behaviors.formatterPirobox = {
    attach: function(context, settings) {
      // Visibility handling.
      if ($('.pirobox-entity-style').length) {
        // Handle 'Pirobox image style', 'Gallery (image grouping)',
        // 'Gallery image random' and 'Gallery covering'.
        if ($('.pirobox-entity-style').val() == 'hide') {
          $('.pirobox-image-style').parent().hide();
          $('.pirobox-gallery-grouping').parent().hide();
          $('.pirobox-gallery-random').parent().hide();
          $('.pirobox-gallery-covering').parent().hide();
          $('.pirobox-gallery-covering').val('0');
        }
        // Handle 'Gallery custom'.
        if ($('.pirobox-gallery-grouping').val() != 'custom') {
          $('.pirobox-gallery-custom').parent().hide();
        }
        // Handle 'Gallery covering '.
        if ($('.pirobox-entity-style').val() == 'link') {
          $('.pirobox-gallery-covering').parent().hide();
          $('.pirobox-gallery-covering').val('0');
        }
      }
      // Visibility handling.
      if ($('.pirobox-gallery-grouping').length) {
        // Handle 'Pirobox image style' and 'Gallery custom'.
        if ($('.pirobox-gallery-grouping').val() == 'none' || $('.pirobox-entity-style').val() == 'hide') {
          $('.pirobox-gallery-custom').parent().hide();
        }
        // Handle 'Gallery image random' and 'Gallery covering'.
        if ($('.pirobox-gallery-grouping').val() == 'none') {
          $('.pirobox-gallery-random').parent().hide();
          $('.pirobox-gallery-covering').parent().hide();
          $('.pirobox-gallery-covering').val('0');
        }
      }
      // Actions on 'Content image style' select.
      $('.pirobox-entity-style').change(function() {
        // Handle 'Pirobox image style', 'Gallery (image grouping)', 'Gallery custom',
        // 'Gallery image random' and 'Gallery covering'.
        if ($('.pirobox-entity-style').val() == 'hide') {
          $('.pirobox-image-style').parent().hide();
          $('.pirobox-gallery-grouping').parent().hide();
          $('.pirobox-gallery-custom').parent().hide();
          $('.pirobox-gallery-random').parent().hide();
          $('.pirobox-gallery-covering').parent().hide();
          $('.pirobox-gallery-covering').val('0');
        }
        else {
          $('.pirobox-image-style').parent().show();
          $('.pirobox-gallery-grouping').parent().show();
          // Handle 'Gallery custom'.
          if ($('.pirobox-gallery-grouping').val() == 'custom') {
            $('.pirobox-gallery-custom').parent().show();
          }
          // Handle 'Gallery image random' and 'Gallery covering'.
          if ($('.pirobox-gallery-grouping').val() != 'none') {
            $('.pirobox-gallery-random').parent().show();
            $('.pirobox-gallery-covering').parent().show();
          }
        }
        // Handle 'Gallery covering '.
        if ($('.pirobox-entity-style').val() == 'link') {
          $('.pirobox-gallery-covering').parent().hide();
          $('.pirobox-gallery-covering').val('0');
        }
        // Handle 'Gallery covering '.
        if ($('.pirobox-gallery-grouping').val() == 'none') {
          $('.pirobox-gallery-covering').parent().hide();
          $('.pirobox-gallery-covering').val('0');
        }
      });
      // Actions on 'Gallery (image grouping)' select.
      // Unbind is nessecary otherwise the change action fired up
      // many times.
      $('.pirobox-gallery-grouping').unbind().change(function() {
        if ($('.pirobox-gallery-grouping').val() == 'none') {
          $('.pirobox-gallery-grouping').val('none');
          // Handle 'Gallery covering'.
          $('.pirobox-gallery-covering').parent().hide();
          $('.pirobox-gallery-covering').val('0');
          // Handle 'Gallery image random'.
          $('.pirobox-gallery-random').parent().hide();
          $('.pirobox-gallery-random').attr('checked', false);
        }
        if ($('.pirobox-gallery-grouping').val() != 'none') {
          // Handle 'Gallery covering'.
          $('.pirobox-gallery-covering').parent().show();
          // Handle 'Gallery image random'.
          $('.pirobox-gallery-random').parent().show();
          //
          $('.pirobox-gallery-covering').parent().show();
        }
        // Handle 'Custom gallery'.
        if ($('.pirobox-gallery-grouping').val() == 'custom') {
          $('.pirobox-gallery-custom').parent().show();
          $('.pirobox-gallery-custom').addClass('required');
          // Make required information visible.
          $('.pirobox-gallery-custom').prev('label').append('<span class="form-required" title="' + Drupal.t("This field is required.") + '">*</span>');
        }
        else {
          $('.pirobox-gallery-custom').parent().hide();
          $('.pirobox-gallery-custom').val('');
          $('.pirobox-gallery-custom').removeClass('error');
          // Remove required information.
          var parE = $('.pirobox-gallery-custom').prev();
          $(parE = ' .form-required').remove();
        }
        // Handle 'Gallery covering '.
        if ($('.pirobox-entity-style').val() == 'link') {
          $('.pirobox-gallery-covering').parent().hide();
          $('.pirobox-gallery-covering').val('0');
        }
      });
    }
  };
})(jQuery);
