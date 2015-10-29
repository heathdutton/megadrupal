/**
 * @file
 * Handles link replacement for elf module.
 */

(function ($) {
  Drupal.behaviors.elf = {
    attach: function(context) {
      $(document).ready(function() {
        // Find all external links and let them open in a new window.
        $("a.elf-external").each(function() {
          $(this).click(function() {
            window.open($(this).attr("href"));
            return false;
          });
        });
      });
    }
  };
})(jQuery);