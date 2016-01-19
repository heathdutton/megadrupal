/**
 * @file
 * JavaScript for the preview modal of the embedded iframe.
 */

/**
 * If jQuery exists, we're outside of the WYSIWYG editor iframe.
 */
if (typeof jQuery !== 'undefined') {
  (function ($) {
    Drupal.behaviors.embed_external_preview = {
      attach: function (context, settings) {
        $('iframe.embed-external').iFrameResize({
          checkOrigin: false,
          heightCalculationMethod: 'bodyScroll',
          sizeHeight: true,
          sizeWidth: true
        });
      }
    };
  })(jQuery);
}
