(function($) {
  Drupal.behaviors.share_light_tracking = {};
  Drupal.behaviors.share_light_tracking.attach = function(context, settings) {
    $('.share-light li a').on('click', function(event) {
      if (typeof ga !== 'undefined') {
        var $link = $(event.target);
        var channel = $link.data('share') ? $link.data('share') : event.target.title;
        var target = "";
        if (typeof settings.share_light !== undefined) {
          target = settings.share_light["share_url"];
        }
        ga('send', 'event', 'share', channel, target);
        ga('send', 'social', channel, 'share', target);
      }
    });
  }
})(jQuery);
