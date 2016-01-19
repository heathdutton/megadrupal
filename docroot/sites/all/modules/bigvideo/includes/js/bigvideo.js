(function ($) {
  Drupal.behaviors.bigvideo = {
    attach: function(context, settings) {
      $('body').once('bigvideo-once', function() {
        var sources = [];
        if (settings.bigvideo.links.mp4) {
          sources.push({ type: 'video/mp4', src: settings.bigvideo.links.mp4 })
        }
        if (settings.bigvideo.links.webm) {
          sources.push({ type: 'video/webm', src: settings.bigvideo.links.webm })
        }
        if (sources.length > 0) {
          var BV = new $.BigVideo();
          BV.init();
          BV.show(sources, { ambient: true });
        }
        else {
          console.error(Drupal.t('Sources array does not contain any values.'));
        }
      });
    }
  }
})(jQuery);