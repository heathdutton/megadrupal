/**
 * @file
 * Handles statement creation for Vimeo player interaction.
 */

(function ($) {

  Drupal.tincanapi.vimeo = {
    getIframe: function (id) {
      return $('#' + id);
    },
    getVideoPlayer: function(id) {
      var iframe = Drupal.tincanapi.vimeo.getIframe(id)[0];
      var player = $f(iframe);

      return player;
    },
    checkup: function (id) {
      var player = Drupal.tincanapi.vimeo.getVideoPlayer(id);
      var video_id = Drupal.tincanapi.vimeo.getIframe(id).data('video_id');
      var info = Drupal.tincanapi.vimeo.info[video_id];

      player.api('getCurrentTime', function (time, id) {
        var time = Math.round(time * 100) / 100,
          diff = time - info.diff;

        if(info.state == "complete") {
          if(diff == 0) {
            // Still complete.
          }
          else {
            Drupal.tincanapi.vimeo.trackEvent('paused', id, null, time);
            info.state = "paused";
          }
        }
        else if(info.state == "paused") {
          if(diff == 0) {
            // Paused.
          }
          else if(diff && Math.floor(diff) == 0) {
            Drupal.tincanapi.vimeo.trackEvent('play', id, time, null);
            info.state = "play";
          }
          else {
            Drupal.tincanapi.vimeo.trackEvent('skipped', id, info.oldTime, time);
            Drupal.tincanapi.vimeo.trackEvent('paused', id, null, time);
          }
        }
        else if(info.state == "play") {
          if(diff == 0) {
            if(time == info.totalDuration) {
              Drupal.tincanapi.vimeo.trackEvent('paused', id, null, time);
              Drupal.tincanapi.vimeo.trackEvent('complete', id, null, time);
              info.state = "complete";
            }
            else {
              Drupal.tincanapi.vimeo.trackEvent('paused', id, null, time);
              info.state = "paused";
            }
          }
          else if(diff && Math.floor(diff) == 0) {
            // Still playing.
          }
          else {
            Drupal.tincanapi.vimeo.trackEvent('paused', id, null, info.oldTime);
            Drupal.tincanapi.vimeo.trackEvent('skipped', id, info.oldTime, time);
            Drupal.tincanapi.vimeo.trackEvent('paused', id, null, time);
            Drupal.tincanapi.vimeo.trackEvent('play', id, time, null);
          }
        }

        info.diff = time;
        info.oldTime = time;
      });
    },
    trackEvent: function (verb, id, start, end) {
      var player = Drupal.tincanapi.vimeo.getVideoPlayer(id);
      var video_id = Drupal.tincanapi.vimeo.getIframe(id).data('video_id');
      var info = Drupal.tincanapi.vimeo.info[video_id];

      var data = {
        module: 'vimeo',
        verb: verb,
        id: info.url,
        title: info.title,
        duration: info.duration,
        referrer: Drupal.settings.tincanapi.currentPage
      };

      if(start != null) {
        data["start_time"] = start;
      }

      if(end != null) {
        data["end_time"] = end;
      }

      Drupal.tincanapi.track(data);

      // Create Watched event.
      if(verb == "play") {
        info.time = start;
      }

      if(verb == "paused" && info.time != null) {
        Drupal.tincanapi.vimeo.trackEvent("watched", id, info.time, end);
        info.time = null;
      }
    },
    info: new Array()
  };

  Drupal.behaviors.tincanapiVimeo = {
    attach: function (context) {
      $('.media-vimeo-video', context).not('.tincan-processed').each(function () {
        var iframe = $(this).find('iframe');
        var id = iframe.attr('title');

        iframe.attr('id', id);
        iframe.attr('src', iframe.attr('src') + "&player_id=" + id);

        if (!id) {
          return;
        }

        var player = Drupal.tincanapi.vimeo.getVideoPlayer(id);
        player.addEvent('ready', function() {
          player.api('getVideoUrl', function(url) {
            var regExp = /http:\/\/(www\.)?vimeo.com\/(\d+)($|\/)/;
            var match = url.match(regExp);

            if (match) {
              var video_id = match[2];
              Drupal.tincanapi.vimeo.getIframe(id).data('video_id', video_id);

              player.api('getDuration', function (duration) {
                $.ajax({
                  url: 'http://vimeo.com/api/v2/video/' + video_id + '.json',
                  dataType: 'json',
                  success: function(data) {
                    data[0]['time'] = null;
                    data[0]['state'] = "paused";
                    data[0]['oldDiff'] = 0;
                    data[0]['diff'] = 0;
                    data[0]["totalDuration"] = duration;

                    Drupal.tincanapi.vimeo.info[video_id] = data[0];

                    setInterval(function() {
                      Drupal.tincanapi.vimeo.checkup(id);
                    }, 400);

                  }
                });
              });
            }
          });
        });

        $(this).addClass('tincan-processed');
      });
    }
  };

})(jQuery);
