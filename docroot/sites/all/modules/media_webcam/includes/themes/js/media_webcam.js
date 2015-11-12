
/**
 * @file media_webcam/includes/themes/js/media_webcam.js
 *
 * Display the Media: Webcam flash uploader.
 */

(function($) {
  // Define Drupal.mediaWebcam.
  Drupal.mediaWebcam = Drupal.mediaWebcam || {};

  // Send a record event to the flash.
  Drupal.mediaWebcam.recordClick = function($element) {
      var webcamFlash = document.getElementById($element.data('flashId'));
      webcamFlash.recordClick();
  }

  // Send a stop event to the flash.
  Drupal.mediaWebcam.stopClick = function($element) {
      var webcamFlash = document.getElementById($element.data('flashId'));
      var red5 = webcamFlash.stopClick();
      $($element.data('red5_textfield')).val(red5);
  }

  Drupal.mediaWebcam.stopTimer = function(textfield, filename) {
      $(textfield).val(filename);
  }

  // Send a play event to the flash.
  Drupal.mediaWebcam.playClick = function($element) {
      var webcamFlash = document.getElementById($element.data('flashId'));
      webcamFlash.playClick();
  }

  // Send a snapshot event to the flash.
  Drupal.mediaWebcam.snapClick = function($element) {
      var webcamFlash = document.getElementById($element.data('flashId'));
      var img = webcamFlash.snapImage();
      $($element.data('red5_snap_textarea')).val(img);
  }

  // Add the flash to the page.
  Drupal.behaviors.mediaWebcamAddFlash = function(context) {
    for (var flashId in Drupal.settings.mediaWebcam.flash) {
      // Bind the record & stop buttons to the flash trigger behavior.
      $('#' + Drupal.settings.mediaWebcam.flash[flashId].wrapperId + ' .media-webcam-record:not(.mediaWebcamAddFlash-processed)', context).each(function() {
        $(this).addClass('mediaWebcamAddFlash-processed')
          .bind('click', function(e) {
            Drupal.mediaWebcam.recordClick($(this));
          })
          .data('flashId', flashId);
      });
      $('#' + Drupal.settings.mediaWebcam.flash[flashId].wrapperId + ' .media-webcam-stop:not(.mediaWebcamAddFlash-processed)', context).each(function() {
        $(this).addClass('mediaWebcamAddFlash-processed')
          .bind('click', function(e) {
            Drupal.mediaWebcam.stopClick($(this));
          })
          .data('flashId', flashId);
        $(this).data('red5_textfield', '#' + Drupal.settings.mediaWebcam.flash[flashId].wrapperId + ' .media-webcam-red5');
      });
      $('#' + Drupal.settings.mediaWebcam.flash[flashId].wrapperId + ' .media-webcam-play:not(.mediaWebcamAddFlash-processed)', context).each(function() {
        $(this).addClass('mediaWebcamAddFlash-processed')
          .bind('click', function(e) {
            Drupal.mediaWebcam.playClick($(this));
          })
          .data('flashId', flashId);
      });
      $('#' + Drupal.settings.mediaWebcam.flash[flashId].wrapperId + ' .media-webcam-snap:not(.mediaWebcamAddFlash-processed)', context).each(function() {
        $(this).addClass('mediaWebcamAddFlash-processed')
          .bind('click', function(e) {
            Drupal.mediaWebcam.snapClick($(this));
          })
          .data('flashId', flashId);
        $(this).data('red5_snap_textarea', '#' + Drupal.settings.mediaWebcam.flash[flashId].wrapperId + ' .media-webcam-snap-textarea');
      });
      $('#' + Drupal.settings.mediaWebcam.flash[flashId].wrapperId + ':not(.mediaWebcamAddFlash-processed)', context).each(function() {
        $(this).addClass('mediaWebcamAddFlash-processed');

        // Replace the Alternate div with the Flash, using swfobject.
        var flashvars = {
          id: flashId,
          width: Drupal.settings.mediaWebcam.flash[flashId].width,
          height: Drupal.settings.mediaWebcam.flash[flashId].height,
          connectUrl: Drupal.settings.mediaWebcam.flash[flashId].connectUrl,
          baseFilename: Drupal.settings.mediaWebcam.flash[flashId].baseFilename,
          maxDuration: Drupal.settings.mediaWebcam.flash[flashId].maxDuration * 1000,
          textfield: '#' + Drupal.settings.mediaWebcam.flash[flashId].wrapperId + ' .media-webcam-red5',
          stopTimerFunction: 'Drupal.mediaWebcam.stopTimer'
        };
        var params = {
          menu: "false",
          scale: "noScale",
          allowFullscreen: "true",
          allowScriptAccess: "always",
          bgcolor: "#FFFFFF"
        };
        var attributes = {
          id: flashId
        };
        swfobject.embedSWF(Drupal.settings.mediaWebcam.webcamPath, flashId, Drupal.settings.mediaWebcam.flash[flashId].width, Drupal.settings.mediaWebcam.flash[flashId].height, "10.0.0", Drupal.settings.mediaWebcam.expressInstallPath, flashvars, params, attributes);
      });

    }
  }

})(jQuery);
