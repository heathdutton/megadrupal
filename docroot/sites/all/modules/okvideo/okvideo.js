/**
 * @file
 * Okvideo.js.
 *
 * Adds settings for OKVideo.
 */

 var Drupal = Drupal || {};

(function($, Drupal){
    "use strict";

    Drupal.behaviors.okVideo = {
      attach: function(context, settings) {

        // If OKVideo loaded and settings available, add settings.
        if ($.fn.okvideo && settings.okvideo) {
          $.okvideo({
            video: settings.okvideo.okvideo_id,
            volume: settings.okvideo.okvideo_vol,
            loop: settings.okvideo.okvideo_loop,
            captions: settings.okvideo.okvideo_captions,
            annotations: settings.okvideo.okvideo_annotations,
            autoplay: settings.okvideo.okvideo_autoplay,
            hd: settings.okvideo.okvideo_hd,
            adproof: settings.okvideo.okvideo_adproof,
          });
        }
      }
    };
})(jQuery, Drupal);
