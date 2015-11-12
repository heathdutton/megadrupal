/**
 * @file
 * Provides JavaScript additions to the managed file field type.
 *
 * This file provides progress bar support (if available), popup windows for
 * file previews, and disabling of other file fields during Ajax uploads (which
 * prevents separate file fields from accidentally uploading files).
 */

(function ($) {

/**
 * Attach behaviors to managed file element upload fields.
 */
Drupal.behaviors.fffFileValidateAutoAttach = {
  attach: function (context, settings) {
    if (settings.fff && settings.fff.elements) {
      $.each(settings.fff.elements, function(selector) {
        var extensions = settings.fff.elements[selector];
        $(selector, context).bind('change', {extensions: extensions}, Drupal.file.validateDisallowedExtension);
      });
    }
  },
  detach: function (context, settings) {
    if (settings.fff && settings.fff.elements) {
      $.each(settings.fff.elements, function(selector) {
        $(selector, context).unbind('change', Drupal.file.validateDisallowedExtension);
      });
    }
  }
};

/**
 * File upload utility functions.
 */
Drupal.file = Drupal.file || {
  /**
   * Client-side file input validation of file extensions.
   */
  validateDisallowedExtension: function (event) {
    if (this.value.length > 0) {
      // Remove any previous errors.
      $('.file-upload-js-error').remove();

      // Add client side validation for the input[type=file].
      var extensionPattern = event.data.extensions.replace(/,\s*/g, '|');
      if (extensionPattern.length > 1) {
        var acceptableMatch = new RegExp('\\.(' + extensionPattern + ')$', 'gi');
        if (acceptableMatch.test(this.value)) {
          var error = Drupal.t("The selected file %filename cannot be uploaded. Files with the following extensions are not allowed: %extensions.", {
            // According to the specifications of HTML5, a file upload control
            // should not reveal the real local path to the file that a user
            // has selected. Some web browsers implement this restriction by
            // replacing the local path with "C:\fakepath\", which can cause
            // confusion by leaving the user thinking perhaps Drupal could not
            // find the file because it messed up the file path. To avoid this
            // confusion, therefore, we strip out the bogus fakepath string.
            '%filename': this.value.replace('C:\\fakepath\\', ''),
            '%extensions': extensionPattern.replace(/\|/g, ', ')
          });
          $(this).closest('div.form-managed-file').prepend('<div class="messages error file-upload-js-error" aria-live="polite">' + error + '</div>');
          this.value = '';
          return false;
        }
      }
    }
  }
};

})(jQuery);
