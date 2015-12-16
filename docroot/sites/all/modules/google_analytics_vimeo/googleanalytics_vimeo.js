/**
 * @file
 * Googleanalytics_vimeo.js.
 */

(function ($) {

  'use strict';

  Drupal.behaviors.GoogleAnalyticsVimeo = {
    attach: function (context, settings) {
      var vimeo_iframes = {};
      vimeo_iframes = $('iframe[src*="player.vimeo.com"]');
      $.each(vimeo_iframes, function (index, iframe) {
        var isrc = $(iframe).attr('src');
        if (!$(iframe).hasClass('google-analytics-vimeo-processed')) {

          iframe.setAttribute('id', 'vimeo-player-' + index);

          var enable_api = '?api=1&player_id=vimeo-player-' + index;

          if (isrc.indexOf('?') !== -1) {
            enable_api = '&api=1&player_id=vimeo-player-' + index;
          }

          $(iframe).addClass('google-analytics-vimeo-processed');
          iframe.setAttribute('src', isrc + enable_api);
          iframe.setAttribute('data-progress', true);
          iframe.setAttribute('data-seek', true);
          iframe.setAttribute('data-bounce', true);
        }
      });
      window.vimeoGAJS.init();
      $.each(window.vimeoGAJS.eventMarker, function (index, iframe) {
        iframe.progress25 = !Drupal.settings.googleanalytics_vimeo.progress25;
        iframe.progress50 = !Drupal.settings.googleanalytics_vimeo.progress50;
        iframe.progress75 = !Drupal.settings.googleanalytics_vimeo.progress75;
        iframe.videoPlayed = !Drupal.settings.googleanalytics_vimeo.videoPlayed;
        iframe.videoPaused = !Drupal.settings.googleanalytics_vimeo.videoPaused;
        iframe.videoResumed = !Drupal.settings.googleanalytics_vimeo.videoResumed;
        iframe.videoSeeking = !Drupal.settings.googleanalytics_vimeo.videoSeeking;
        iframe.videoCompleted = !Drupal.settings.googleanalytics_vimeo.videoCompleted;
      });
    }
  };
})(jQuery);
