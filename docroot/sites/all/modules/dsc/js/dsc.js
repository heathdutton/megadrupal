/**
 * @file
 * Fill in placeholder to match defaults.
 */
(function ($) {
  Drupal.behaviors.DSC = {
    attach: function (context, settings) {
      $('#dsc-admin-settings #edit-default').change(function () {
        $('#edit-details input').attr('placeholder', $(this).val());
      });
    }
  };

})(jQuery);