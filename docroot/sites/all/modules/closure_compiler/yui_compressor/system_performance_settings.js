/**
 * @file
 * YUI Compressor form system_performance_settings JS file.
 */

(function($) {
  /**
   * Registers a function to run when the DOM is ready
   */
  $(document).ready(function() {
    if (!$('#edit-closure-compiler-service-yui-compressor').attr('checked')) {
      $('#edit-yui-compressor').hide();
    }

    /**
     * Bind an event to the 'edit-closure-compiler-service-yui-compressor' checkbox
     */
    $('#edit-closure-compiler-service-yui-compressor').change(function() {
      if ($(this).attr('checked')) {
        $('#edit-yui-compressor').show();
      } else {
        $('#edit-yui-compressor').hide();
      }
    });

    return;
  });

}(jQuery));
