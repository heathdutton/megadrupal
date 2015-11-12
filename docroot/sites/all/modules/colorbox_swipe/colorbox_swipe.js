(function($) {
  /**
   * Initialization
   */
  Drupal.behaviors.colorbox_swipe = {
    /**
     * Run Drupal module JS initialization.
     *
     * @param context
     * @param settings
     */
    attach: function(context, settings) {
      $(document).ready(function() {
        // Global context!
        $("#colorbox *", context).bind(
                'swipeleft',
                function(e) {
                  // Disable selection.
                  $(this).disableSelection();
                  $.colorbox.next();
                }).bind(
                'swiperight',
                function(e) {
                  // Disable selection.
                  $(this).disableSelection();
                  $.colorbox.prev();
                });
      });
    }
  }
})(jQuery);
