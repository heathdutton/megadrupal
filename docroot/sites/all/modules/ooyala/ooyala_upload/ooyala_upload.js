/**
 * @file
 * Ooyala upload widget JavaScript code.
 */
Drupal.ooyala = Drupal.ooyala || {'listeners': {}, 'onCreateHandlers': {}, 'players': {} };

(function ($) {
  /**
   * The behavior for an Ooyala Uploader button.
   */
  Drupal.behaviors.ooyalaUploader = {
    attach: function (context, settings) {
      // TODO we should be able to remove this hard-coded ID.
      $('#selectAsset').once('ooyala-uploader', function() {
        $('#selectAsset').change(Drupal.ooyala.onFileSelected);

        // Hide unused elements until they are needed.
        $('#ooyala-message').hide();
        $("#progress-bar").hide();
      });

      if (!Drupal.settings.ooyala || Drupal.ooyala.uploader) {
        return;
      }

      // Detect if this browser supports file slicing. We explicitly only check
      // for slice and not mozSlice / webkitSlice because the ingestion library
      // only works with slice.
      var uploaderType = typeof Blob == "function" && typeof Blob.prototype.slice == "function" ? "HTML5" : "Flash";
      if (uploaderType == "HTML5") {
        Drupal.ooyala.uploader = new OoyalaUploader({
          uploaderType: uploaderType
        });
      }
      else {
        Drupal.ooyala.uploader = new OoyalaUploader({
          uploaderType: uploaderType,
          swfUploader: Drupal.ooyala.swfUploader()
        });
      }

      // Set up all of our event listeners.
      Drupal.ooyala.uploader.on("uploadProgress", Drupal.ooyala.onProgress);
      Drupal.ooyala.uploader.on("uploadError", Drupal.ooyala.onUploadError);
      Drupal.ooyala.uploader.on("embedCodeReady", Drupal.ooyala.onEmbedCodeReady);
      Drupal.ooyala.uploader.on("uploadComplete", Drupal.ooyala.onUploadComplete);
    }
  };

  /**
   * Event handler for when a file has been selected to upload.
   */
  Drupal.ooyala.onFileSelected = function(event) {
    var file = event.target.files[0];

    // The user cancelled the file selection.
    if (typeof file == "undefined") {
      return;
    }

    $('#ooyala-message').empty().hide();
    if (!file.type.match(/video|audio/)) {
      $('#ooyala-message').append(Drupal.t('The file selected is not detected as being a video or audio file.')).show();
      return;
    }

    Drupal.ooyala.uploader.file = file;

    $('#ooyala-message').fadeIn(600);
    $('.ooyala-button-container').removeClass('ooyala-finished');

    Drupal.ooyala.startUpload();
  };

  /**
   * Event handler for when a file has been selected to upload using SWFupload.
   */
  Drupal.ooyala.onFileSelectedFlash = function(numFilesSelected, numFilesQueued, numFilesInQueue) {
    Drupal.ooyala.uploader.file = this.getFile(0);
    Drupal.ooyala.startUpload();
  };

  /**
   * Event handler for monitoring upload progress.
   */
  Drupal.ooyala.onProgress = function(assetID, progress) {
    // Still uploading, so update our progress bar.
    if (progress < 100) {
      $('#progress-bar').progressbar('value', progress);
    }
  };

  /**
   * Event handler for when an asset upload is complete.
   */
  Drupal.ooyala.onUploadComplete = function(assetID) {
    $("#progress-bar").fadeOut(600, function() {
      $('#ooyala-message').empty().append(Drupal.t('Upload of <span class="ooyala-filename">!file</span> complete.', {'!file': Drupal.ooyala.uploader.file.name}));
      $("#progress-bar").progressbar('destroy');
      $('#selectAssetWrapper').css('height', 22).css('margin-left', 0);
      $('#selectAssetWrapper').animate({ opacity: 1 });
    });

    $('.ooyala-button-container').addClass('ooyala-finished');
    $('#node-form input.form-submit, #node-form input.form-button').removeAttr('disabled');
  };

  Drupal.ooyala.onUploadError = function(assetID, type, fileName, statusCode, message) {
    if (message) {
      $('#ooyala-message').empty().append(Drupal.t('Upload Error: @error', { '@error': message }));
    }
  };

  /**
   * Event callback for when an asset has an embed code ready to be uploaded
   * to.
   */
  Drupal.ooyala.onEmbedCodeReady = function(assetID) {
    $('.ooyala-embed-code-input').val(assetID);

    // This kicks off actually uploading the asset after we have reserved
    // an embed code in the backlot.
    $('#node-form input.form-submit, #node-form input.form-button').attr('disabled', 'disabled');
  };

  /**
   * Start the upload of an asset.
   */
  Drupal.ooyala.startUpload = function() {
    // Grab the title and create a new embed code for the asset in the Ooyala
    // backlot.
    var title = $('#edit-title').val();
    var uploaderType = Drupal.ooyala.uploader.uploaderType;

    if (title === '') {
      $('#ooyala-message').empty().append(Drupal.t('A title is required in order to upload this asset.'));
      $('#ooyala-message').fadeIn(600);
      $('#edit-title').addClass('error');

      // With a regular file upload field, we have to clear it so we trigger
      // the file selected event when the user fixes the title and re-selects
      // their file. The only way to clear a file element is to destroy it and
      // recreate it.
      if (uploaderType == "HTML5") {
        $('#selectAssetWrapper').html('<input id="selectAsset" type="file" />');
        $('#selectAsset').change(Drupal.ooyala.onFileSelected);
      }

      return false;
    }

    var options = {
      name: title,
      assetCreationUrl: Drupal.settings.ooyala.endpoint + '/create',
      assetStatusUpdateUrl: Drupal.settings.ooyala.endpoint + '/complete/assetID',
      assetUploadingUrl: Drupal.settings.ooyala.endpoint + '/load/assetID/uploading_urls'
    };

    $('#selectAssetWrapper').animate({ opacity: 0 }, {
      complete: function() {
        $(this).css('margin-left', -9999).css('height', 0);
        $('#ooyala-message').html(Drupal.t('Uploading <span class="ooyala-filename">!file</span>...', {'!file': Drupal.ooyala.uploader.file.name}));
        $('#ooyala-message').fadeIn(600);
        $("#progress-bar").progressbar();
        $("#progress-bar").fadeIn(600);
      }
    });

    if (uploaderType == "HTML5") {
      Drupal.ooyala.uploader.uploadFile(Drupal.ooyala.uploader.file, options);
    }
    else {
      Drupal.ooyala.uploader.uploadFileUsingFlash(options);
    }
  };

  /**
   * Return a new SWFUpload object for use with browsers that don't support file
   * slicing.
   */
  Drupal.ooyala.swfUploader = function() {
    var settings;
    var baseUrl = window.location.protocol + '//' + window.location.host + Drupal.settings.basePath;
    var flashUrl = baseUrl + 'sites/all/libraries/backlot-ingestion-library/lib/swfupload.swf';
    var buttonImageUrl = baseUrl + Drupal.settings.ooyala.buttonImageUrl;

    settings = {
      file_queue_limit: 1,
      file_upload_limit: 1,
      file_dialog_complete_handler: Drupal.ooyala.onFileSelectedFlash,
      flash_url: flashUrl,
      button_placeholder_id: "selectAsset",
      button_image_url: buttonImageUrl,
      button_height: 22,
      button_width: 61
    };

    return new SWFUpload(settings);
  };

})(jQuery);

