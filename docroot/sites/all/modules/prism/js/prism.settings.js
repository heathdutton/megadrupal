/**
 * @file
 * Basic prism settings.
 */

(function ($) {
  Drupal.behaviors.prism = {
    attach: function(context, settings) {
      // Check if an ajax call has been made.
      $(document).ajaxComplete(function() {
        // Make sure we highlight any newly added content.
        // @todo, target specific content wrapper(s)?
        Prism.highlightAll();
      });
    }
  };
})(jQuery);
