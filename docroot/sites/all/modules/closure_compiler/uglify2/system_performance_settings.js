/**
 * @file
 * Uglify2 form system_performance_settings JS file.
 */

(function($) {
  /**
   * Registers a function to run when the DOM is ready
   */
  $(document).ready(function() {
    if (!$('#edit-closure-compiler-service-uglify2').attr('checked')) {
      $('#edit-uglify2').hide();
    }

    /**
     * Bind an event to the 'edit-closure-compiler-service-uglify2' checkbox
     */
    $('#edit-closure-compiler-service-uglify2').change(function() {
      if ($(this).attr('checked')) {
        $('#edit-uglify2').show();
      } else {
        $('#edit-uglify2').hide();
      }
    });

    return;
  });

}(jQuery));
