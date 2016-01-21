(function($) {
  /**
   * Initialize easy zoom functionality.
   */
  Drupal.behaviors.easyzoom = {
    attach: function(context, settings) {

      // Instantiate EasyZoom instances.
      var $easyzoom = $('.easyzoom').easyZoom();

      // Setup thumbnails.
      var api1 = $easyzoom.filter('.easyzoom--with-thumbnails').data('easyZoom');
      $('.easyzoom-thumbnails').on('click', 'a', function(e) {
        var $this = $(this);

        e.preventDefault();

        // Use EasyZoom's `swap` method
        api1.swap($this.data('standard'), $this.attr('href'));
      });

    }
  }

})(jQuery);
