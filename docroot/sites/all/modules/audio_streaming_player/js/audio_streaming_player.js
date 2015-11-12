/**
 * Audio Streaming Player.
 */

(function($) {
  'use strict';
  Drupal.behaviors.audiostreamingplayer = {
    attach: function(context, settings) {
      var pause_ = true;
      var startmessage = Drupal.t('Click Play to start listening');
      $("#responsecontainer").html(startmessage);
      var inCourse = "";
      var stream = {
        // Atributtes: title and stream url.
        title: startmessage,
        mp3: Drupal.settings.audiostreamingplayer.audio_streaming_player_stream_url
      },
      ready = false;

      $("#audio_streaming_player_black").jPlayer({
        ready: function(event) {
          ready = true;
          // If the variable auto-play is on, then starts playing from the beginning.
          if (Drupal.settings.audiostreamingplayer.audio_streaming_player_auto_play === 1) {
            $(this).jPlayer("setMedia", stream).jPlayer("play");
            pause_ = false;
          }

        },
        pause: function() {
          // Pause audio playback.
          $(this).jPlayer("clearMedia");
          pause_ = true;
        },
        play: function() {
          // To avoid both jPlayers playing together.
          $(this).jPlayer("pauseOthers");
          pause_ = false;
          inCourse = "#responsecontainer_black";
        },
        error: function(event) {
          if (ready && event.jPlayer.error.type === $.jPlayer.error.URL_NOT_SET) {

            pause_ = false;
            // Setup the media stream again and play it.
            $(this).jPlayer("setMedia", stream).jPlayer("play");

          }
        },
        // Container where are located the player.
        cssSelectorAncestor: "#audio_streaming_player_black_container",
        solution: 'flash, html',
        // Swf Path.
        swfPath: Drupal.settings.audiostreamingplayer.swf,
        // Allowed Formats.
        supplied: "mp3,oga",
        preload: "none",
        wmode: "window",
        keyEnabled: true
      });
      $("#audio_streaming_player_circular").jPlayer({
        ready: function(event) {
          ready = true;

          // If the variable auto-play is on, then starts playing from the beginning.
          if (Drupal.settings.audiostreamingplayer.audio_streaming_player_auto_play === 1) {
            $(this).jPlayer("setMedia", stream).jPlayer("play");
            pause_ = false;
          }

        },
        pause: function() {
          // Pause audio playback.
          $(this).jPlayer("clearMedia");
          pause_ = true;
        },
        play: function() {
          // To avoid both jPlayers playing together.
          $(this).jPlayer("pauseOthers");
          pause_ = false;
          inCourse = "#responsecontainer_circular";
        },
        error: function(event) {

          if (ready && event.jPlayer.error.type === $.jPlayer.error.URL_NOT_SET) {

            pause_ = false;
            // Setup the media stream again and play it.
            $(this).jPlayer("setMedia", stream).jPlayer("play");

          }
        },
        // Container where are located the player.
        cssSelectorAncestor: "#audio_streaming_player_circular_container",
        solution: 'flash, html',
        // Swf path.
        swfPath: Drupal.settings.audiostreamingplayer.swf,
        // Allowed Formats.
        supplied: "mp3,oga",
        preload: "none",
        wmode: "window",
        keyEnabled: true
      });
      $("#audio_streaming_player_text").jPlayer({
        ready: function(event) {
          ready = true;
          // If the variable auto-play is on, then starts playing from the beginning.
          if (Drupal.settings.audiostreamingplayer.audio_streaming_player_auto_play === 1) {
            $(this).jPlayer("setMedia", stream).jPlayer("play");
            pause_ = false;
          }

        },
        play: function() {
          // To avoid both jPlayers playing together.
          $(this).jPlayer("pauseOthers");
          pause_ = false;
          inCourse = "#responsecontainer_text";
        },
        pause: function() {
          // Pause audio playback.
          $(this).jPlayer("clearMedia");
          pause_ = true;
        },
        error: function(event) {
          if (ready && event.jPlayer.error.type === $.jPlayer.error.URL_NOT_SET) {
            pause_ = false;
            // Setup the media stream again and play it.
            $(this).jPlayer("setMedia", stream).jPlayer("play");

          }
        },
        // Container where are located the player.
        cssSelectorAncestor: "#audio_streaming_player_text_container",
        solution: 'flash, html',
        // Swf path.
        swfPath: Drupal.settings.audiostreamingplayer.swf,
        // Allowed Formats.
        supplied: "mp3,oga",
        preload: "none",
        wmode: "window",
        keyEnabled: true
      });
      // JplayerInspector for each Player.
      $("#inspector_text").jPlayerInspector({jPlayer: $("#audio_streaming_player_text")});
      $("#inspector_circular").jPlayerInspector({jPlayer: $("#audio_streaming_player")});
      $("#inspector_black").jPlayerInspector({jPlayer: $("#audio_streaming_player_black")});

      // Get the metadata from the stream.
      var refreshId = setInterval(function() {

        if (!pause_) {
          var parameters = {
            "audio_streaming_player_stream_url": Drupal.settings.audiostreamingplayer.audio_streaming_player_stream_url
          };
          // Ajax call to get the metadata.
          $.ajax({
            data: parameters,
            url: Drupal.settings.audiostreamingplayer.path + '/NowPlaying/as_getnowplaying.php',
            type: 'post',
            beforeSend: function() {

            },
            success: function(response) {
              // Load the result in the container.
              $(inCourse).html(response);
            }
          });
        }
        else {
          $(inCourse).html(startmessage);
        }
      }, 6000);
    }
  };
}(jQuery));
