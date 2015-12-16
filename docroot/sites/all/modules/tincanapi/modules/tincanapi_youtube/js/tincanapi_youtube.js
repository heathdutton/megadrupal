/**
 * @file
 * Handles statement creation for YouTube player interaction.
 */

(function ($) {

  var tag = document.createElement('script');

  tag.src = "//www.youtube.com/iframe_api";
  var firstScriptTag = document.getElementsByTagName('script')[0];
  firstScriptTag.parentNode.insertBefore(tag, firstScriptTag);

  window.onYouTubeIframeAPIReady = function() {
    $(document).ready(function () {
      $('.media-youtube-video').not('.tincan-processed').each(function () {
        Drupal.tincanapi.youtube.processVideo($(this));
      });
    });
  };

  Drupal.behaviors.tincanapiYouTube = {
    attach: function (context) {
      // The YouTube API isn't ready when the page is loaded.
      if (typeof YT !== 'undefined') {
        $('.media-youtube-video', context).not('.tincan-processed').each(function () {
          Drupal.tincanapi.youtube.processVideo($(this));
        });
      }
    }
  };

  Drupal.tincanapi.youtube = {
    processVideo: function(video) {
      if(typeof YT.Player === "undefined") {
        return;
      }

      var iframe = video.find('iframe');
      var id;

      if (iframe.attr('id')) {
        id = iframe.attr('id');
      }
      else {
        id = Drupal.tincanapi.youtube.getRandomId();
        iframe.attr('id', id);
      }

      var player = new YTPlayer(id, {
        events: {
          'onReady': Drupal.tincanapi.youtube.onPlayerReady,
          'onSkipped': Drupal.tincanapi.youtube.onSkipped,
          'onPaused': Drupal.tincanapi.youtube.onPaused,
          'onPlay': Drupal.tincanapi.youtube.onPlay,
          'onComplete': Drupal.tincanapi.youtube.onComplete
        }
      });

      video.addClass('tincan-processed');
    },
    onPlayerReady: function(event) {
      var players = Drupal.tincanapi.youtube.players;

      var video_data = event.target.getVideoData();
      var video_id = video_data.video_id;

      $.getJSON('https://gdata.youtube.com/feeds/api/videos/' + video_id + '?v=2&alt=jsonc',
        function (data) {
          players[video_id] = new Array();
          players[video_id]['title'] = data.data.title;
          players[video_id]['duration'] = data.data.duration;
          players[video_id]['time'] = null;
          players[video_id]['state'] = 'initialized';
        }
      );
    },
    onSkipped: function(video_id, from, to) {
      var players = Drupal.tincanapi.youtube.players;
      if (players[video_id] !== undefined) {
        Drupal.tincanapi.youtube.trackEvent("skipped", video_id, from, to);
      }
    },

    onPlay: function(video_id, start) {
      var players = Drupal.tincanapi.youtube.players;
      if (players[video_id] !== undefined) {
        Drupal.tincanapi.youtube.trackEvent("play", video_id, start, null);
        players[video_id]['time'] = start;
      }
    },

    onPaused: function(video_id, time) {
      var players = Drupal.tincanapi.youtube.players;
      if (players[video_id] !== undefined) {
        Drupal.tincanapi.youtube.trackEvent("paused", video_id, null, time);

        if(players[video_id]['time'] != null) {
          Drupal.tincanapi.youtube.trackEvent("watched", video_id, players[video_id]['time'], time);
          players[video_id]['time'] = null;
        }
      }
    },

    onComplete: function(video_id, end) {
      var players = Drupal.tincanapi.youtube.players;
      if (players[video_id] !== undefined) {
        Drupal.tincanapi.youtube.trackEvent("complete", video_id, null, end);
      }
    },

    trackEvent: function (verb, video_id, start_time, end_time) {
      var players = Drupal.tincanapi.youtube.players;

      data = {
        module: 'youtube',
        verb: verb,
        id: video_id,
        title: players[video_id]['title'],
        duration: players[video_id]['duration'],
        referrer: Drupal.settings.tincanapi.currentPage
      };

      if (start_time != null) {
        data['start_time'] = start_time;
      }

      if (end_time != null) {
        data['end_time'] = end_time;
      }

      Drupal.tincanapi.track(data);
    },
    getRandomId: function() {
      var id = '';

      var min = 97;
      var max = 122;

      for (var i = 0; i < 10; i++) {
        var random = Math.floor(Math.random() * (max - min + 1)) + min;
        id += String.fromCharCode(random);
      }

      return 'youtube-' + id;
    },
    players: new Array()
  };

})(jQuery);
