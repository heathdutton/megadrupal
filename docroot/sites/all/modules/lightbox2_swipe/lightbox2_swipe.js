(function($) {
  /**
   * Initialization
   */
  Drupal.behaviors.lightbox2_swipe = {
    /**
     * Run Drupal module JS initialization.
     * 
     * @param context
     * @param settings
     */
    attach: function(context, settings) {
      $(document).ready(function() {
        // Global context!
        $("#lightbox *", context).bind(
                'swipeleft',
                function(e) {
                  // Disable selection.
                  $(this).disableSelection();
                  $(this).find('#nextLink').remove();
                  $(this).find('#prevLink').remove();
                  if ((Lightbox.total > 1 && ((Lightbox.isSlideshow && Lightbox.loopSlides) || (!Lightbox.isSlideshow && Lightbox.loopItems)))
                          || Lightbox.activeImage != (Lightbox.total - 1)) {
                    Lightbox.changeData(Lightbox.activeImage + 1);
                  }
                }).bind(
                'swiperight',
                function(e) {
                  // Disable selection.
                  $(this).disableSelection();
                  $(this).find('#nextLink').remove();
                  $(this).find('#prevLink').remove();
                  if ((Lightbox.total > 1 && ((Lightbox.isSlideshow && Lightbox.loopSlides) || (!Lightbox.isSlideshow && Lightbox.loopItems)))
                          || Lightbox.activeImage !== 0) {
                    Lightbox.changeData(Lightbox.activeImage - 1);
                  }
                });
      });
    }
  }
})(jQuery);
