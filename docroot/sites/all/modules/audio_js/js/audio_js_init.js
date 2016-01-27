/**
 * @file
 *
 * Replaces all instances of the <audio> tag using audio.js.
 */

(function ($) {
  Drupal.behaviors.audio_js = {
    attach: function(context) {
      var SWFpath = Drupal.settings.audio_js.swf;
      var as = audiojs.createAll({
        css: false,
        swfLocation: SWFpath,
        init: function()
          {
            $(".audiojs-item").removeClass("audiojs-item");
          }
        }, $(".audiojs-item"));
    }
  };
})(jQuery);
