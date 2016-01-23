/*global Drupal: false, jQuery: false */
/*jslint devel: true, browser: true, maxerr: 50, indent: 2 */
(function ($) {
  "use strict";

  Drupal.behaviors.imageeditor_scriptcam = {};
  Drupal.behaviors.imageeditor_scriptcam.attach = function(context, settings) {
    $('#imageeditor_scriptcam').scriptcam({
      path: Drupal.settings.imageeditor.scriptcam.path,
      useMicrophone: false,
      width: 640,
      height: 480,
      onWebcamReady: Drupal.imageeditor_scriptcam.onWebcamReady,
      onError: Drupal.imageeditor_scriptcam.onError
    });
  };

  Drupal.imageeditor_scriptcam = Drupal.imageeditor_scriptcam || {};
  Drupal.imageeditor_scriptcam = function() {
    return {
      onWebcamReady: function(cameraNames, camera, microphoneNames, microphone, volume) {
        alert(cameraNames);
        alert($.scriptcam.version);
        alert($.scriptcam.getFrameAsBase64());
        $.each(cameraNames, function(index, text) {
          $('#imageeditor_scriptcam_cams').append( $('<option></option>').val(index).html(text) );
        });
        $('#imageeditor_scriptcam_cams').val(camera);
        $.each(microphoneNames, function(index, text) {
          $('#imageeditor_scriptcam_mics').append( $('<option></option>').val(index).html(text) );
        });
        $('#imageeditor_scriptcam_mics').val(microphone);
      },
      onError: function(errorId,errorMsg) {
        alert(errorMsg);
      }
    };
  }();

})(jQuery);
