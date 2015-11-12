/**
 * @file
 * Closure Compiler form system_performance_settings JS file.
 */

(function($) {
  /**
   * Registers a function to run when the DOM is ready
   */
  $(document).ready(function() {
    if (!$('#edit-preprocess-js').attr('checked') || !$('#edit-closure-compiler-js-optimization').attr('checked')) {
      $('#edit-closure-compiler').hide();
    }

    /**
     * Bind an event to the 'edit-preprocess-js' checkbox
     */
    $('#edit-preprocess-js').change(function() {
      if ($(this).attr('checked') && $('#edit-closure-compiler-js-optimization').attr('checked')) {
        $('#edit-closure-compiler').show();
      } else {
        $('#edit-closure-compiler').hide();
      }
    });

    /**
     * Bind an event to the 'edit-closure-compiler-js-optimization' checkbox
     */
    $('#edit-closure-compiler-js-optimization').change(function() {
      if ($(this).attr('checked') && $('#edit-preprocess-js').attr('checked')) {
        $('#edit-closure-compiler').show();
      } else {
        $('#edit-closure-compiler').hide();
      }
    });

    return;
  });

}(jQuery));
