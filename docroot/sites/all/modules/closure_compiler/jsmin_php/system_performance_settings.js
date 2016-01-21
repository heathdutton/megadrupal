/**
 * @file
 * JSMin-PHP form system_performance_settings JS file.
 */

(function($) {
  /**
   * Registers a function to run when the DOM is ready
   */
  $(document).ready(function() {
    if (!$('#edit-closure-compiler-service-jsmin-php').attr('checked')) {
      $('#edit-jsmin-php').hide();
    }

    /**
     * Bind an event to the 'edit-closure-compiler-service-jsmin-php' checkbox
     */
    $('#edit-closure-compiler-service-jsmin-php').change(function() {
      if ($(this).attr('checked')) {
        $('#edit-jsmin-php').show();
      } else {
        $('#edit-jsmin-php').hide();
      }
    });

    return;
  });

}(jQuery));
