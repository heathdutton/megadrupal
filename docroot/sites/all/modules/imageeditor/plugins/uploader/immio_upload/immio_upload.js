/*global Drupal: false, jQuery: false */
/*jslint devel: true, browser: true, maxerr: 50, indent: 2 */
(function ($) {
  "use strict";

  Drupal.imageeditor.uploaders.immio_upload = Drupal.imageeditor.uploaders.immio_upload || {};
  Drupal.imageeditor.uploaders.immio_upload.initialize = function($imageeditor_div) {
    $imageeditor_div.find('.immio-upload').not('.imageeditor-processed').addClass('imageeditor-processed').click(function(event) {
      event.preventDefault();
      event.stopPropagation();
      var data = $imageeditor_div.data();
      var codename = $(this).data('codename');
      $.ajax({
        type: 'POST',
        url: Drupal.settings.imageeditor[codename].parameters.callback_url,
        async: false,
        data: {'url': data.url},
        success: function(response) {
          if (response.hasOwnProperty('error')) {
            alert(Drupal.t('Failed to upload the image') + ': ' + response.error);
          }
          else {
            data.url = response.url;
            $imageeditor_div.data(data);
            alert(Drupal.t('Successfully uploaded image') + ': ' + response.url);
          }
        },
        error: function(msg) {
          alert(Drupal.t('Failed to upload the image') + ': ' + msg);
        }
      });
    });
  }

})(jQuery);
