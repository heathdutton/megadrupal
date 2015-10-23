(function($) {
  "user strict";

  imce.uploadValidate = function (data, form, options) {
    var $files = $('#edit-imce .plupload_filelist li .plupload_file_name');

    if (!$files.length) {
      return false;
    }

    if (imce.conf.extensions != '*') {
      var validated = true;

      $files.each(function() {
        var file = $.trim($(this).text());
        var ext  = file.substr(file.lastIndexOf('.') + 1);

        if ((' ' + imce.conf.extensions + ' ').indexOf(' ' + ext.toLowerCase() + ' ') == -1) {
          validated = false;
          return false;
        }
      });

      if(!validated) {
        return imce.setMessage(Drupal.t('Only files with the following extensions are allowed: %files-allowed.', {'%files-allowed': imce.conf.extensions}), 'error');
      }
    }

    options.url = imce.ajaxURL('upload');
    imce.fopLoading('upload', true);

    return true;
  };

  imce.oldUploadSettings = imce.uploadSettings;

  // Settings for upload
  imce.uploadSettings = function () {
    var settings = this.oldUploadSettings();

    settings.complete = function () {
      imce.fopLoading('upload', false);

      $('.plupload_buttons').show();
      $('.plupload_upload_status').hide();

      setTimeout(function () {
        $('#edit-imce').pluploadQueue().splice();
      }, 200);
    };

    return settings;
  };
}) (jQuery);
