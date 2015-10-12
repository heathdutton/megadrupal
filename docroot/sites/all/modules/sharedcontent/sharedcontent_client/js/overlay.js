/**
 * @file
 * Behaviors for the Shared Content overlay.
 */

(function ($) {

  Drupal.behaviors.SharedContentOverlay = {
    attach: function (context, settings) {
        if (!$.isFunction($.colorbox)) {
          return;
        }
        // Only enable the overlay if the window width is big enough.
        if (typeof window.matchMedia == 'function'
          && typeof settings.sharedcontent != "undefined"
          && typeof settings.sharedcontent.overlay_media_query != "undefined") {

          var mq = window.matchMedia(settings.sharedcontent.overlay_media_query);
          if (!mq.matches) {
            // window width is too small, abort.
            return;
          }
        }

        $('.sharedcontent-overlay a.sharedcontent-origin', context)
          .once('init-colorbox-load', function () {
            var parts = $(this)[0].href.match(/([^\?#]+)(\?.+)?(#.+)?/);
            var url = parts[1];
            url += parts[2] ? parts[2] + '&sc[overlay]=true' : '?sc[overlay]=true';
            url += parts[3] ? parts[3] : '';
            var params = {
              'href': url,
              'iframe': true,
              'width': '80%',
              'height': '100%'
            };
            $(this).colorbox($.extend({}, settings.colorbox, params));
          });
    }
  };

})(jQuery);
