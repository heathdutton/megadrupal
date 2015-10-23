/**
 * @file
 * Provide summaries for vertical tabs in the content forms.
 */
(function ($) {
  Drupal.behaviors.lyris_contentFieldsetSummeries = {
    attach: function (context) {
      // Set required fields notice.
     return $(context).hasRequired();
    }
  }
})(jQuery);
