(function ($) {
  jQuery(document).ready(function(context) {
    if (jQuery("body", window.parent.document).length > 0) {
      // Close all colorboxes on page.
      window.parent.jQuery.colorbox.close();
    }
  });
})(jQuery);