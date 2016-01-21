/**
 * @file
 * Contains javascript for selfi.
 */

(function ($) {

  var userstream;
  Drupal.selfi = Drupal.selfi || {};

  Drupal.theme.prototype.showMessage = function (msg, value) {
    var divClass = (value) ? 'status' : 'error';
    var message = '<div class="messages ' + divClass + '">' + Drupal.t(msg);
    message += '</div>';
    return message;
  };
  // Show error message if there was problem while accessing webcam.
  Drupal.selfi.errorCallback = function(e) {
    var error = 'There was a problem while accessing the webcam';
    $(Drupal.theme('showMessage', error, 0)).insertAfter('.video-container .actions').delay(10000).hide('slow');
  };
  // Play video.
  Drupal.selfi.successCallback = function(stream) {
    userstream = stream;
    $('#selfi_video').attr('src', window.URL.createObjectURL(stream));
    $('#selfi_video').get(0).play();
  };
  // Stop video.
  Drupal.selfi.stopCam = function() {
    $('#selfi_video').attr('src', '');
    userstream.stop();
    userstream = null;
  };
  // Initialize navigator.getMedia object depending upon the browser.
  Drupal.selfi.WebRtcInit = function() {
    navigator.getMedia = (
      navigator.getUserMedia ||
      navigator.webkitGetUserMedia ||
      navigator.mozGetUserMedia
    );
  };

  Drupal.behaviors.takeSelfi = {
    attach: function(context, settings) {
      $('#startbutton', context).once('start-cam', function() {
        Drupal.selfi.WebRtcInit();
        // Start the camera.
        $('#startbutton').click(function (e) {
          $('#takepicture').attr('disabled', false);
          $('#startbutton').attr('disabled', true);
          var video_constraints = {
            mandatory: { },
            optional: []
          };
          navigator.getMedia({
            audio: false,
            video: video_constraints
          }, Drupal.selfi.successCallback, Drupal.selfi.errorCallback);
        });
      });

      $('#takepicture', context).once('take-selfie', function() {
        $('#takepicture').click(function (e) {
          $('#takepicture').attr('disabled', true);
          $('#startbutton').attr('disabled', false);
          if ($('#edit-picture-upload').length) {
            $('#edit-picture-upload').val("");
          };
          canvas = $('#canvas').get(0);
          canvas.width = settings.selfi.width;
          canvas.height = settings.selfi.height;
          canvas.getContext('2d').drawImage(selfi_video, 0, 0, settings.selfi.width, settings.selfi.height);
          var dataURL = canvas.toDataURL('image/png');

          $.ajax({
            url : settings.basePath + 'selfi/upload-pic',
            dataType : "json",
            data: {'img_data' : dataURL, 'selfi_token' : settings.selfi.selfiToken},
            type: 'POST',
            /* complete runs on both if error comes or successful */
            complete: function(response, status) {
              Drupal.selfi.stopCam();
            },
            success: function(data) {
              if (data.status) {
                $(Drupal.theme('showMessage',data.status.msg, data.status.value)).insertAfter('.video-container .actions').delay(10000).hide('slow');
                if (data.status.file && $('#selfi-data').length) {
                  $('#selfi-data').val(data.status.file);
                }
              }
            },
          });
        });
      });
    }
  }

})(jQuery);
