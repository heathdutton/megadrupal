(function ($) {

/**
 * Enable/Disable the 'Compile configuration' checkbox on the performance page.
 */
Drupal.enableCompileConfigurationCheckbox = function() {
  // Toggle the display as necessary when the checkbox is clicked.
  $('#edit-inject-dump').click( function () {
    $('#edit-inject-compile').attr('disabled', !this.checked);
    $('.form-item-inject-compile').toggleClass('form-disabled', !this.checked);
  });
};

})(jQuery);
