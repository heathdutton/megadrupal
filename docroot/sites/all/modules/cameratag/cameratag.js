/**
 * @file
 * Callback to read information form the cameratag api.
 *
 * Save the video id when recording is finished into the field value.
 */

(function ($) {
  "use strict";

  Drupal.behaviors.cameratagrecord = {
    attach: function (context, settings) {

      CameraTag.observe(settings.cameratag.camera_id, 'published', function () {
        var myCamera = CameraTag.cameras[settings.cameratag.camera_id];
        var camera_tag_upload_video_uuid = document.getElementById('cameratag_video_uuid');

        var video = myCamera.getVideo();
        console.log(video);
        camera_tag_upload_video_uuid.value = video.uuid;
      });

      // Make camera tag strings translatable.
      CT_i18n[0] = Drupal.t('To record this video using your mobile phone please visit <<url>> in your mobile browser');
      CT_i18n[1] = Drupal.t('Your mobile device does not support video uploading');
      CT_i18n[2] = Drupal.t('Please make sure you have Flash Player 11 or higher installed');
      CT_i18n[3] = Drupal.t('Unable to embed video recorder. Please make sure you have Flash Player 11 or higher installed');
      CT_i18n[4] = Drupal.t('choose a method below to submit your video');
      CT_i18n[5] = Drupal.t('record from webcam');
      CT_i18n[6] = Drupal.t('upload a file');
      CT_i18n[7] = Drupal.t('record from phone');
      CT_i18n[8] = Drupal.t('wave to the camera');
      CT_i18n[9] = Drupal.t('recording in');
      CT_i18n[10] = Drupal.t('uploading...');
      CT_i18n[11] = Drupal.t('click to stop recording');
      CT_i18n[12] = Drupal.t('click to skip review');
      CT_i18n[13] = Drupal.t('Accept');
      CT_i18n[14] = Drupal.t('Re-record');
      CT_i18n[15] = Drupal.t('Review Recording');
      CT_i18n[16] = Drupal.t('please wait while we push pixels');
      CT_i18n[17] = Drupal.t('published');
      CT_i18n[18] = Drupal.t('Enter your <b>mobile phone number</b> below and we will text you a link for mobile recording');
      CT_i18n[19] = Drupal.t('Send Mobile Link');
      CT_i18n[20] = Drupal.t('cancel');
      CT_i18n[21] = Drupal.t('Check your phone for mobile recording instructions');
      CT_i18n[22] = Drupal.t('or point your mobile browser to');
      CT_i18n[23] = Drupal.t('drop file to upload');
      CT_i18n[24] = Drupal.t('sending your message');
      CT_i18n[25] = Drupal.t('please enter your phone number!');
      CT_i18n[26] = Drupal.t('that does not appear to be a valid phone number');
      CT_i18n[27] = Drupal.t('Unable to send SMS.');
      CT_i18n[28] = Drupal.t('No Camera Detected');
      CT_i18n[29] = Drupal.t('No Microphone Detected');
      CT_i18n[30] = Drupal.t('Camera Access Denied');
      CT_i18n[31] = Drupal.t('Lost connection to server');
      CT_i18n[32] = Drupal.t('Playback failed');
      CT_i18n[33] = Drupal.t('Unable To Connect');
      CT_i18n[34] = Drupal.t('Unable to publish your recording.');
      CT_i18n[35] = Drupal.t('Unable to submit form data.');
      CT_i18n[36] = Drupal.t('uploading your video');
      CT_i18n[37] = Drupal.t('buffering video playback');
      CT_i18n[38] = Drupal.t('publishing');
      CT_i18n[39] = Drupal.t('connecting...');
      CT_i18n[40] = Drupal.t('negotiating firewall...');
      CT_i18n[41] = Drupal.t('Oh No! It looks like your browser paused the recorder');
    }
  };
})(jQuery);
