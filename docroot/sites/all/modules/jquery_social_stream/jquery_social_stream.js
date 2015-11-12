(function ($) {
  Drupal.behaviors.jQuerySocialStream = {
    attach: function (context, settings) {
      // Render all social streams.
      if (typeof settings.jQuerySocialStream != 'undefined') {
        for (var id in settings.jQuerySocialStream) {
          $('#' + id).dcSocialStream(settings.jQuerySocialStream[id]);
        }
      }
    }
  };
})(jQuery);
