/**
 * @file
 * Adds an interface between the media recorder jQuery plugin and the drupal media module.
 */

(function ($) {
  'use strict';

  Drupal.MediaRecorderFlash = (function () {
    var settings = Drupal.settings.mediaRecorder.settings;
    var origin = window.location.origin || window.location.protocol + "//" + window.location.hostname + (window.location.port ? ':' + window.location.port : '');
    var recordingName;
    var statusInterval;
    var $element;
    var $statusWrapper;
    var $previewWrapper;
    var $video;
    var $audio;
    var $meter;
    var $startButton;
    var $recordButton;
    var $playButton;
    var $stopButton;
    var $settingsButton;
    var $videoButton;
    var $audioButton;

    /**
     * Set status message.
     */
    function setStatus(message) {
      $element.trigger('status', message);
    }

    /**
     * Toggle to recording preview.
     */
    function showSettings() {
      FWRecorder.showPermissionWindow({permanent: true});
    }

    /**
     * Send a blob as form data to the server. Requires jQuery 1.5+.
     */
    function sendBlob(blob) {
      setStatus('Uploading, please wait...');

      // Create formData object.
      var formData = new FormData();
      var req = new XMLHttpRequest();
      formData.append("mediaRecorder", blob);

      // Send file.
      req.addEventListener("load", transferComplete, false);
      req.open('POST', origin + Drupal.settings.basePath + 'media_recorder/record/file', true);
      req.send(formData);
      function transferComplete(evt) {
        var file = JSON.parse(req.response);
        $element.trigger('uploadFinished', file);
      }
    }

    /**
     * Stop recording and trigger stopped event.
     */
    function start() {
      showSettings();
    }

    /**
     * Stop recording and trigger stopped event.
     */
    function stop() {
      FWRecorder.stopPlayBack();
      FWRecorder.stopRecording();
    }

    /**
     * Start recording and trigger recording event.
     */
    function record() {
      FWRecorder.record(recordingName);
    }

    /**
     * Initialize all control buttons.
     */
    function initializeButtons() {

      // Click handler for enable button.
      $startButton.bind('click', function (event) {
        event.preventDefault();
        $startButton[0].disabled = true;
        start();
        setStatus('Allow access to your mic in the settings panel.');
      });

      // Click handler for record button.
      $recordButton.bind('click', function (event) {
        event.preventDefault();
        $recordButton[0].disabled = true;
        $recordButton.hide();
        $playButton.hide();
        $stopButton.show();
        record();
      });

      // Click handler for stop button.
      $stopButton.bind('click', function (event) {
        event.preventDefault();
        $stopButton.hide();
        $recordButton.show();
        $playButton.show();
        stop();
      });

      // Click handler for play button.
      $playButton.bind('click', function (event) {
        event.preventDefault();
        $playButton.hide();
        $stopButton.show();
        FWRecorder.playBack(recordingName);
      });

      // Click handler for settings button.
      $settingsButton.bind('click', function (event) {
        event.preventDefault();
        showSettings();
      });
    }

    /**
     * Initialize recorder.
     */
    function initializeEvents() {

      // FWRecorder ready event.
      $element.bind('ready', function (event) {

        // Fix for weird FWRecorder use of window instead of document.
        window['flashRecorder'] = document['flashRecorder'];

        FWRecorder.connect('flashRecorder', 0);
        FWRecorder.recorderOriginalWidth = 1;
        FWRecorder.recorderOriginalHeight = 1;

        if (!FWRecorder.isMicrophoneAccessible()) {
          $startButton.show();
        }
      });

      // FWRecorder microphone_connected event.
      $element.bind('microphone_user_request', function (event) {
        FWRecorder.showPermissionWindow();
      });

      // FWRecorder microphone_connected event.
      $element.bind('microphone_connected', function (event) {
        FWRecorder.configure(22, 50, 0, 4000);
        $startButton.hide();
        $recordButton.show();
        setStatus('Press record to start recording.');
        FWRecorder.observeLevel();
      });

      // FWRecorder microphone_not_connected event.
      $element.bind('microphone_not_connected', function (event) {
        $startButton.show();
        $startButton[0].disabled = false;
        $recordButton.hide();
        setStatus('Click \'Start\' to enable your mic & camera.');
      });

      // FWRecorder no_microphone_found event.
      $element.bind('no_microphone_found', function (event, name, data) {
        $startButton.show();
        $startButton[0].disabled = false;
        $recordButton.hide();
        setStatus('No mic found. Click \'Start\' to enable your mic & camera.');
      });

      // FWRecorder permission_panel_closed event.
      $element.bind('permission_panel_closed', function (event) {
        FWRecorder.defaultSize();
      });

      // FWRecorder observing_level event.
      $element.bind('observing_level', function (event, name, data) {
        $meter.show();
        $meter.height(20);
      });

      // FWRecorder observing_level_stopped event.
      $element.bind('observing_level_stopped', function (event, name, data) {
        $meter.hide();
      });

      // FWRecorder microphone_level event.
      $element.bind('microphone_level', function (event, name, data) {
        var level = data * 100;
        $meter.width(level + '%');
        if (data * 100 <= 70) {
          $meter.css({
            'background': 'green'
          });
        }
        else if (level > 70 && level < 85) {
          $meter.css({
            'background': 'yellow'
          });
        }
        else if (level >= 85) {
          $meter.css({
            'background': 'red'
          });
        }
      });

      // FWRecorder recording event.
      $element.bind('recording', function (event, data) {
        var currentSeconds = 0;
        var timeLimitFormatted = millisecondsToTime(new Date(parseInt(settings.time_limit, 10) * 1000));

        setStatus('Recording 00:00 (Time Limit: ' + timeLimitFormatted + ')');
        statusInterval = setInterval(function () {
          currentSeconds = currentSeconds + 1;
          var currentMilliSeconds = new Date(currentSeconds * 1000);
          var time = millisecondsToTime(currentMilliSeconds);
          setStatus('Recording ' + time + ' (Time Limit: ' + timeLimitFormatted + ')');

          if (currentSeconds >= settings.time_limit) {
            stop();
          }
        }, 1000);
      });

      // FWRecorder recording_stopped event.
      $element.bind('recording_stopped', function (event) {
        var blob = FWRecorder.getBlob(recordingName);
        clearInterval(statusInterval);
        sendBlob(blob);
      });

      // FWRecorder recording_stopped event.
      $element.bind('stopped', function (event) {
        $playButton.show();
        $stopButton.hide();
      });

      $element.bind('uploadFinished', function (event, data) {
        $element.find('.media-recorder-fid').val(data.fid);
        $recordButton[0].disabled = false;
        setStatus('Press record to start recording.');
      });

      $element.bind('status', function (event, msg) {
        $statusWrapper.text(msg);
      });
    }

    /**
     * Convert milliseconds to time format.
     */
    function millisecondsToTime(milliSeconds) {
      var milliSecondsDate = new Date(milliSeconds);
      var mm = milliSecondsDate.getMinutes();
      var ss = milliSecondsDate.getSeconds();
      if (mm < 10) {
        mm = "0" + mm;
      }
      if (ss < 10) {
        ss = "0" + ss;
      }
      return mm + ':' + ss;
    }

    /**
     * Initialize recorder.
     */
    function init(element) {
      $element = $(element);
      $statusWrapper = $element.find('.media-recorder-status');
      $previewWrapper = $element.find('.media-recorder-preview');
      $video = $element.find('.media-recorder-video');
      $audio = $element.find('.media-recorder-audio');
      $meter = $element.find('.media-recorder-meter');
      $startButton = $element.find('.media-recorder-enable');
      $recordButton = $element.find('.media-recorder-record');
      $stopButton = $element.find('.media-recorder-stop');
      $playButton = $element.find('.media-recorder-play');
      $settingsButton = $element.find('.media-recorder-settings');
      $videoButton = $element.find('.media-recorder-enable-video');
      $audioButton = $element.find('.media-recorder-enable-audio');
      recordingName = 'audio';

      // Append flash recorder div.
      $element.append('<div id="flash-wrapper"><div id="flashcontent"><p>Your browser must have JavaScript enabled and the Adobe Flash Player installed.</p></div></div>');

      // Initialize flash recorder.
      window.fwr_event_handler = function (eventName) {
        $element.trigger(eventName, arguments);
      };
      swfobject.embedSWF(
        Drupal.settings.basePath + Drupal.settings.mediaRecorder.flashurl + '/html/recorder.swf',
        'flashcontent',
        1,
        1,
        '11.0.0',
        '',
        {},
        {},
        {'id': 'flashRecorder', 'name': 'flashRecorder'}
      );

      // Initial state.
      $recordButton.hide();
      $recordButton.hide();
      $stopButton.hide();
      $playButton.hide();
      $stopButton.before($playButton);
      $video.hide();
      $audio.hide();
      $meter.hide();
      $videoButton.hide();
      $audioButton.hide();

      // Show file preview if file exists.
      if (Drupal.settings.mediaRecorder.file) {
        var file = Drupal.settings.mediaRecorder.file;
        switch (file.type) {
          case 'video':
            $previewWrapper.show();
            $video.show();
            $audio.hide();
            $video[0].src = Drupal.settings.mediaRecorder.file.url;
            $video[0].muted = '';
            $video[0].controls = 'controls';
            $video[0].load();
            break;
          case 'audio':
            $previewWrapper.show();
            $audio.show();
            $video.hide();
            $audio[0].src = Drupal.settings.mediaRecorder.file.url;
            $audio[0].muted = '';
            $audio[0].controls = 'controls';
            $audio[0].load();
            break;
        }
      }

      initializeButtons();
      initializeEvents();
      setStatus('Click \'Start\' to enable your mic & camera.');
    }

    return {
      init: init,
      start: start,
      record: record,
      stop: stop
    };
  })();
})(jQuery);
