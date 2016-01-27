(function ($) {
  Drupal.behaviors.background_audio = {};
  Drupal.behaviors.background_audio.attach = function (context, settings) {
    if ($('div.background-audio').length) {
      return false;
    }

    var div = document.createElement('div'),
    audioElement = document.createElement('audio'),
    source;
    div.className = 'background-audio background-audio-' + Drupal.settings.background_audio.settings.position;

    /* Do not preload media - this prevents media loading if no autoplay. */
    audioElement.setAttribute('preload', 'none');
    if (Drupal.settings.background_audio.settings.autoplay) {
      audioElement.setAttribute('autoplay', 'autoplay');
    }
    /* If this attribute is set - then the "ended" event doesn't fire after song and it repeats just the first song */
    /*if (Drupal.settings.background_audio.background_audio_loop) {
      audioElement.setAttribute('loop', 'loop');
    }*/
    audioElement.setAttribute('controls', 'controls');

    document.body.appendChild(div);
    div.appendChild(audioElement);

    $.each(Drupal.settings.background_audio.files, function(index, element) {
      source = document.createElement('source');
      source.setAttribute('src', element.src);
      source.setAttribute('title', element.title);
      source.setAttribute('type', element.type);
      audioElement.appendChild(source);
    });

    var features = [];
    $.each(Drupal.settings.background_audio.settings.controls, function(index, element) {
      features.push(index);
    });

    $(audioElement).mediaelementplayer({
      audioWidth: Drupal.settings.background_audio.settings.width,
      audioHeight: Drupal.settings.background_audio.settings.height,
      startVolume: Drupal.settings.background_audio.settings.volume,
      loop: Drupal.settings.background_audio.settings.loop,
      shuffle: Drupal.settings.background_audio.settings.shuffle,
      playlist: Drupal.settings.background_audio.settings.playlist,
      playlistposition: Drupal.settings.background_audio.settings.playlist_position,
      features: features,
      success: function (mediaElement, domObject) {
        // Read data from cookies.
        var src = $.cookie('background_audio_src'),
          state = $.cookie('background_audio_state'),
          currentTime = $.cookie('background_audio_currentTime'),
          volume = $.cookie('background_audio_volume'),
          muted = $.cookie('background_audio_muted'),
          loop = $.cookie('background_audio_loop'),
          shuffle = $.cookie('background_audio_shuffle'),
          playlist = $.cookie('background_audio_playlist');

        if (src != null) {
          //mediaElement.setSrc(src);
          mediaElement.player.playTrackURL(src);
        }
        if (state == 'play') {
          mediaElement.load();
          mediaElement.play();
        }
        else if (state == 'pause') {
          mediaElement.pause();
        }
        if (volume != null) {
          mediaElement.setVolume(volume);
        }
        if (muted == 'true') {
          mediaElement.setMuted(true);
        }
        else if (muted == 'false') {
          mediaElement.setMuted(false);
        }

        mediaElement.addEventListener('play', function () {
          $.cookie('background_audio_state', 'play', {expires: 7, path: Drupal.settings.basePath});
        });
        mediaElement.addEventListener('pause', function () {
          $.cookie('background_audio_state', 'pause', {expires: 7, path: Drupal.settings.basePath});
        });
        mediaElement.addEventListener('volumechange', function () {
          $.cookie('background_audio_volume', mediaElement.volume.toFixed(3), {expires: 7, path: Drupal.settings.basePath});
          $.cookie('background_audio_muted', mediaElement.muted, {expires: 7, path: Drupal.settings.basePath});
        });
        mediaElement.addEventListener('timeupdate', function () {
          $.cookie('background_audio_currentTime', mediaElement.currentTime.toFixed(1), {expires: 7, path: Drupal.settings.basePath});
        });
        mediaElement.addEventListener('loadedmetadata', function () {
          $.cookie('background_audio_src', mediaElement.src, {expires: 7, path: Drupal.settings.basePath});
          if (typeof currentTime != 'undefined') {
            mediaElement.setCurrentTime(currentTime);
            currentTime = undefined;
          }
        });

        $(mediaElement).bind('mep-looptoggle', function (event, loop) {
          $.cookie('background_audio_loop', loop, {expires: 7, path: Drupal.settings.basePath});
        });
        $(mediaElement).bind('mep-shuffletoggle', function (event, shuffle) {
          $.cookie('background_audio_shuffle', shuffle, {expires: 7, path: Drupal.settings.basePath});
        });
        $(mediaElement).bind('mep-playlisttoggle', function (event, playlist) {
          $.cookie('background_audio_playlist', playlist, {expires: 7, path: Drupal.settings.basePath});
        });

        if ((loop == 'false' && mediaElement.player.options.loop) || (loop == 'true' && !mediaElement.player.options.loop)) {
          mediaElement.player.loopToggleClick();
        }
        if ((shuffle == 'false' && mediaElement.player.options.shuffle) || (shuffle == 'true' && !mediaElement.player.options.shuffle)) {
          mediaElement.player.shuffleToggleClick();
        }
        if ((playlist == 'false' && mediaElement.player.options.playlist) || (playlist == 'true' && !mediaElement.player.options.playlist)) {
          mediaElement.player.playlistToggleClick();
        }
      }
    });

    /* Copy padding-top from body. */
    $(div).css('padding-top', $('body').css('padding-top'));
  };
})(jQuery);
