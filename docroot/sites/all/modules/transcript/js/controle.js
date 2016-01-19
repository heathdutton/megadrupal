/**
 * @file
 * Provides Youtube's video transcript highligting.
 */

(function ($) {

  // Create deferred object.
  var YTdeferred = $.Deferred();
  window.onYouTubeIframeAPIReady = function() {
    // Resolve when youtube callback is called.
    // passing YT as a parameter.
    YTdeferred.resolve(window.YT);
  };

  var tag = document.createElement('script');
  tag.src = "https://www.youtube.com/iframe_api";
  var firstScriptTag = document.getElementsByTagName('script')[0];
  firstScriptTag.parentNode.insertBefore(tag, firstScriptTag);

  // 3. This function creates an <iframe> (and YouTube player)
  // after the API code downloads.
  var player;
  var TRANSCRIPT_SHOW = "100";
  var STATE_CHECK = "4000";
  Drupal.behaviors.transcript = {
    attach: function () {

      // Whenever youtube callback was called = deferred resolved
      // your custom function will be executed with YT as an argument.
      YTdeferred.done(function(YT) {

        player = new YT.Player('transcript-youtube-video', {
          events: {
            'onStateChange': onPlayerStateChange
          }
        });

        // 5. The API calls this function when the player's state changes.
        // The function indicates that when playing a video (state=1),
        // the player should play for six seconds and then stop.
        var done = false;
        function onPlayerStateChange(event) {
          startTranscript();
        }

      function startTranscript() {
        setInterval(function(){
          var playerTime = Math.round(player.getCurrentTime());
            if (document.getElementById(playerTime)) {
              $('.transcript-tracks').removeClass('active');
              $('#' + playerTime).addClass('active');
            }
          }, TRANSCRIPT_SHOW);
        }

        $('.transcript-tracks').click(function(){
          var idd = $(this).attr('id');
          var state = player.getPlayerState();
          $('.transcript-tracks').removeClass('active');
          $(this).addClass('active');
          var id_val = idd;
          if(state == 5){
            player.playVideo();
            setTimeout(function() {
              player.seekTo(id_val, true);
            }, STATE_CHECK);
          } else {
            player.seekTo(id_val, true);
          }
        });

      });

    }
  };

})(jQuery);
