/**
 * Pinned Site admin javascript.
 */
(function ($) {
  Drupal.behaviors.pinnedSiteColorpicker = {
    attach: function() {
      $('.pinned_site_colorpicker').each(function(){
        $(this).farbtastic('input[name=' + $(this).data('target') + ']');
      });
    }
  };
 })(jQuery);
