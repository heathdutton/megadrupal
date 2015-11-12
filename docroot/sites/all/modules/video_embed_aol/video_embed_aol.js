/**
 * @file
 * This file contains an AJAX loader for AOL ON video player.
 */

(function($) {

  Drupal.behaviors.video_embed_aol = {
    attach: function (context, settings) {
      /*
       * Load the information about the video and insert video script on it's place.
       */
      if  (typeof settings.video_embed_aol !== 'undefined') {

        var get_videos = function (selector, val) {
          $('#' + selector, context).once (selector, function() {
            $.ajax({
              url: val.api_endpoint_url,
              crossDomain: true,
              dataType: 'json',
              error: function() {
                $('#' + selector).text(Drupal.t('Error'));
              },
              success: function(data) {
                if (typeof data.items !== 'undefined') {
                  if (0 in data.items) {
                    var player = data.items[0].player.source;
                    var src = $(player).attr('src');
                    $.getScript(src);
                    $('#' + selector)[0].innerHTML = player;
                  }
                }
              }
            });
          });
        };
        $.each(settings.video_embed_aol, get_videos);
      }
    }
  };

})(jQuery);
