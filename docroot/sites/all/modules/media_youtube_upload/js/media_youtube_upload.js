(function ($) {

  /**
   * Jquery Plugin to initiate file upload to Youtube.
   */
  $.fn.startUpload = function(data) {
    MediaYoutubeUpload.startUpload($.parseJSON(data));
  };

  var UploadVideo = function() {
    this.videoId = '';
    this.uploadStartTime = 0;
  };

  UploadVideo.prototype.ready = function(accessToken, object) {
    this.accessToken = accessToken;
  };

  /**
   * Uploads a video file to YouTube.
   *
   * @method uploadFile
   * @param {object} file File object corresponding to the video to upload.
   * @param {string} token requested to upload.
   */
  UploadVideo.prototype.uploadFile = function($file, token, fieldNameHelper) {
    var file =  $file[0].files[0];
    var metadata = {
      snippet: {
        title: this.title,
        description: this.description,
        tags: this.tags,
        categoryId: this.categoryId
      },
      status: {
        privacyStatus: this.privacy
      }
    };
    var uploader = new MediaUploader({
      baseUrl: 'https://www.googleapis.com/upload/youtube/v3/videos',
      file: file,
      token: token,
      metadata: metadata,
      params: {
        part: Object.keys(metadata).join(',')
      },
      onError: function(data) {
        var message = data;
        // Assuming the error is raised by the YouTube API, data will be
        // a JSON string with error.message set. That may not be the
        // only time onError will be raised, though.
        try {
          var errorResponse = JSON.parse(data);
          message = errorResponse.error.message;
        } finally {
          alert(message);
        }
      }.bind(this),
      onProgress: function(data) {
          
        var currentTime = Date.now();
        var bytesUploaded = data.loaded;
        var totalBytes = data.total;
        // The times are in millis, so we need to divide by 1000 to get seconds.
        var bytesPerSecond = bytesUploaded / ((currentTime - this.uploadStartTime) / 1000);
        var estimatedSecondsRemaining = Math.round((totalBytes - bytesUploaded) / bytesPerSecond);
        var percentageComplete = Math.ceil((bytesUploaded * 100) / totalBytes);
        
        // Remove preparing upload text
        $('#youtube-cors-progress').html($('#youtube-cors-progress').children());
        // Update the progressbar
        $('#youtube-cors-progress').progressbar({value: percentageComplete});
        
      }.bind(this),
      onComplete: function(data) {
        var uploadResponse = JSON.parse(data);
        // Make sure and use the filename provided by Drupal as it may have
        // been renamed.
        var youtubeID = 'input[name="' + fieldNameHelper.name + '[youtube_id]"]';
        $(youtubeID).val(uploadResponse.id);
        // Re-enable all the submit buttons.
        // form.find('input[type="submit"]').removeAttr('disabled');
        // Find trigger the #ajax method for the upload button that was
        // initially clicked to upload the file.
        // var button_id = $file.attr('id') + "-button";
        var file_id = $file.attr('id');
        // Prevent Drupal from transferring the file twice as part of the
        // form rebuild.
        $('#' + file_id).remove();
        var buttonName = 'input[name="' + fieldNameHelper.name + '[save_button]"]';
        $(buttonName).mousedown();
        

      }.bind(this)
    });
    // This won't correspond to the *exact* start of the upload, but it should be close enough.
    this.uploadStartTime = Date.now();
    uploader.upload();
  };


  Drupal.behaviors.mediaYoutubeUpload = {};
  
  /**
   * Attach behaviors to media Youtube uploader fields.
   */
  Drupal.behaviors.mediaYoutubeUploadValidateAutoAttach = {
    attach: function (context, settings) {
      if (settings.media_youtube_upload) {
        $.each(settings.media_youtube_upload, function(selector) {
          var upload_validators = settings.media_youtube_upload[selector].upload_validators;
          var youtube_field_helper = settings.media_youtube_upload[selector].youtube_field_helper;
          // Only bind on files that haven't been processed yet.
          $(selector, context).once('media-youtube-uploader', function() {
            // Extra variables that we use to open and close the fields. And
            // also to enable and disable the upload button.
            // TODO: See if we can move this functionality to the #states
            // attribute of the form element?
            var fieldsPresent = '#edit-' + youtube_field_helper.id + "-youtube-fields";
            youtube_field_helper.fieldsPresent = fieldsPresent;
            var uploadButton = 'input[name="' + youtube_field_helper.name + '[upload_button]"]';
            youtube_field_helper.uploadButton =  uploadButton;
            // Hide the fields as default.
            $(fieldsPresent).hide();
            $(uploadButton).disabled = true;
            $(uploadButton).addClass("form-button-disabled");
            // Bind change functionality
            $(this).change({upload_validators: upload_validators, field_helper: youtube_field_helper}, MediaYoutubeUpload.getFields);
          });
        });
      }
    },
    detach: function (context, settings) {
      if (settings.media_youtube_upload) {
        $.each(settings.media_youtube_upload, function(selector) {
          // Only bind on files that haven't been processed yet.
          $(selector, context).once('media-youtube-uploader', function() {
            $(this).unbind('change', Drupal.mediaYoutubeUpload.getFields);
          });
        });
      }
    }
  };

  MediaYoutubeUpload = {};

  MediaYoutubeUpload.getFields = function(event) {
    // Remove any previous errors.
    $('.file-upload-js-error').remove();
    
    // Just some logging of the files in the event. This might come in handy
    // when tackling the multiple value field for our widget.
    var files = event.originalEvent.target.files;
    for (var i=0, len=files.length; i<len; i++){
      console.log(files[i].name, files[i].type, files[i].size);
    }

    // Set the variables that are needed for our clientside file validation.
    var allowedExtensions = event.data.upload_validators.allowed_extensions;
    var allowedSize = event.data.upload_validators.max_uploadsize;
    var extensionPattern = allowedExtensions.replace(/,\s*/g, '|');
    var fileSize = event.originalEvent.target.files[0].size;
    var fileNameInput = 'input[name="' + event.data.field_helper.name + '[filename]"]';
    var fileSizeInput = 'input[name="' + event.data.field_helper.name + '[filesize]"]';
    $uploadButton = $(event.data.field_helper.uploadButton);
    $fieldsPresent = $(event.data.field_helper.fieldsPresent);
    var errors = new Array();
    

    // Check if file has correct extension.
    if (extensionPattern.length > 1 && $(this).val().length > 0) {
      var acceptableMatch = new RegExp('\\.(' + extensionPattern + ')$', 'gi');
      if (!acceptableMatch.test($(this).val())) {
        errors.push(Drupal.t("The selected file %filename cannot be uploaded. Only files with the following extensions are allowed: %extensions.", {
          // Extended information about fakepath in the core file module file.js
          '%filename': $(this).val().replace('C:\\fakepath\\', ''),
          '%extensions': extensionPattern.replace(/\|/g, ', ')
        }));
      }
    }

    // Check if filesize is smaller than the allowed size.
    if (fileSize > allowedSize) {
      errors.push(Drupal.t("The selected file %filename is too large. Please select a file that is smaller than %allowedsize.", {
          '%filename': $(this).val().replace('C:\\fakepath\\', ''),
          '%allowedsize': Drupal.mediaYoutubeUpload.makeSizeReadable(allowedSize)
      }));
    }
    // If there are errors on the selected file display them and remove the file
    // from input.
    if (typeof errors !== "undefined" && errors.length !== 0) {
      var errorString = '<div class="messages error file-upload-js-error" aria-live="polite">';
      $.each(errors , function(i, val) { 
        errorString += (errors.length > 1 && i === 0) ? '<ul>' : '';
        errorString += (errors.length > 1) ? '<li>' + val + '</li>' : val;
        errorString += (errors.length > 1 && i === errors.length - 1) ? '</ul>' : '';
      });
      errorString += '</div>';
      // Put the error on the page.
      $(this).closest('div.form-type-media-youtube-upload-upload').prepend(errorString);
      //Remove the value from the file input.
      $(this).val('');
      $(fileNameInput).val('');
      $(fileSizeInput).val('');
      // Hide if we don't have a file.
      // TODO: Move this to #states in the form. But has a bug that needs fixin.
      $fieldsPresent.hide();
      $uploadButton.disabled = true;
      $uploadButton.addClass("form-button-disabled");
    }
    else {
      $(fileNameInput).val($(this).val());
      $(fileSizeInput).val(fileSize);
      // Show the fields if we have a file.
      // TODO: Move this to #states in the form. But has a bug that needs fixin.
      $fieldsPresent.show();
      $uploadButton.disabled = false;
      $uploadButton.removeClass("form-button-disabled");
      // This can be implemented when we start without fields to retrieve the
      // default fields with their default values after file selection.
      //$('input[name="' + getFieldsButtonName + '"]').mousedown();
    }
  }
  
  MediaYoutubeUpload.resetFormElement = function(e) {
    e.wrap('<form>').closest('form').get(0).reset();
    e.unwrap();
  }

  /**
   * Function to convert bytes into a more readable size.
   */
  MediaYoutubeUpload.makeSizeReadable =  function (bytes) {
    var sizes = ['Bytes', 'KB', 'MB', 'GB', 'TB'];
    if (bytes == 0) return '0 Byte';
    var i = parseInt(Math.floor(Math.log(bytes) / Math.log(1024)));
    return Math.round(bytes / Math.pow(1024, i), 2) + ' ' + sizes[i];
  },
  
  /**
   * Receives an XMLHttpRequestProgressEvent and uses it to display current
   * progress if possible.
   *
   * @param event
   *   And XMLHttpRequestProgressEvent object.
   */
  MediaYoutubeUpload.displayProgress = function(el, event) {
    if (event.lengthComputable) {
      percent = Math.floor((event.loaded / event.total) * 100);
      $('#youtube-cors-progress').progressbar({value: percent});
      return true;
    }
  },
  
  /**
   * Function to check if upload was successfull.
   */
  MediaYoutubeUpload.startUpload = function (data) {
    var id = data.id;
    var fieldNameHelper =  Drupal.settings.media_youtube_upload[data.id].youtube_field_helper;
    $file = $(id);
    // Add a placholder for our progress bar.
    $file.hide().after('<div id="youtube-cors-progress" style="width: 270px; float: left;">' + Drupal.t('Preparing upload ...') + '</div>');
    
    var uploadVideo = new UploadVideo();
    uploadVideo.description = data.fields.field_file_youtube_description;
    uploadVideo.tags = data.fields.field_file_youtube_tags;
    uploadVideo.categoryId = data.fields.field_file_youtube_category;
    uploadVideo.title = data.fields.field_file_youtube_title;
    uploadVideo.privacy = data.fields.field_file_youtube_privacy;
    uploadVideo.uploadFile($file, data.token, fieldNameHelper);
  };

})(jQuery);
