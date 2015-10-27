/**
 * @file
 * Inline Styles Cleaner auto download client side logic.
 */

(function($) {
  Drupal.behaviors.InlineStylesCleaner = {
    attach: function(context, settings) {
      if (Drupal.settings.inline_styles_cleaner.css_file.download_needed !== false) {
        window.location.href = '/' + Drupal.settings.inline_styles_cleaner.css_file.download_link + '/' + Drupal.settings.inline_styles_cleaner.css_file.fid;
      }
    }
  }
})(jQuery);
