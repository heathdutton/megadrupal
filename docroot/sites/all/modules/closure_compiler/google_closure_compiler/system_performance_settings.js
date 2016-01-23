/**
 * @file
 * Google Closure Compiler form system_performance_settings JS file.
 */

(function($) {
  /**
   * Registers a function to run when the DOM is ready
   */
  $(document).ready(function() {
    /**
     * Disable the local option if it is not available
     */
    if (!Drupal.settings.google_closure_compiler.local_available) {
      $('#edit-google-closure-compiler-process-method-0').attr('disabled', 'disabled');
    }

    if (!$('#edit-closure-compiler-service-google-closure-compiler').attr('checked')) {
      $('#edit-google-closure-compiler').hide();
    }

    /**
     * Bind an event to the 'edit-closure-compiler-service-google-closure-compiler' checkbox
     */
    $('#edit-closure-compiler-service-google-closure-compiler').change(function() {
      if ($(this).attr('checked')) {
        $('#edit-google-closure-compiler').show();
      } else {
        $('#edit-google-closure-compiler').hide();
      }
    });

    return;
  });

}(jQuery));
