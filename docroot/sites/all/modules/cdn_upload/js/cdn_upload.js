(function($) {
  $(function () {
    // Make sure we have settings
    if (Drupal.settings.cdn_upload) {

      // Iterate through the upload instances, although there should only be one.
      $.each(Drupal.settings.cdn_upload, function(id, settings) {

        // Create the endpoint.
        var endpoint = Drupal.settings.basePath + 'cdn_upload';
        endpoint += '/' + encodeURIComponent(settings.conf);

        // Create the session endpoint.
        var sessionEndpoint = Drupal.settings.basePath + 'cdn_session';
        sessionEndpoint += '/' + encodeURIComponent(settings.conf);

        // Create the uploader.
        var uploader = $('#' + id + '-upload').fineUploaderS3({
          request: {
            endpoint: settings.bucket + ".s3.amazonaws.com",
            accessKey: settings.publicKey
          },

          template: id + "-upload-template",

          // REQUIRED: Path to our local server where requests
          // can be signed.
          signature: {
            endpoint: endpoint
          },

          // OPTIONAL: An endopint for Fine Uploader to POST to
          // after the file has been successfully uploaded.
          // Server-side, we can declare this upload a failure
          // if something is wrong with the file.
          uploadSuccess: {
            endpoint: endpoint + '&success=1'
          },

          // USUALLY REQUIRED: Blank file on the same domain
          // as this page, for IE9 and older support.
          iframeSupport: {
            localBlankPagePath: endpoint
          },

          // optional feature
          chunking: {
            enabled: true
          },

          // optional feature
          resume: {
            enabled: true
          },

          session: {
            endpoint: sessionEndpoint
          },

          // optional feature
          deleteFile: {
            enabled: true,
            method: "DELETE",
            endpoint: Drupal.settings.basePath + 'cdn_upload'
          },

          // optional feature
          validation: {
            itemLimit: 5,
            sizeLimit: settings.maxSize
          },

          thumbnails: {
            placeholders: {
              notAvailablePath: '/' + settings.modulePath + "/assets/not_available-generic.png",
              waitingPath: '/' + settings.modulePath + "/assets/waiting-generic.png"
            }
          }
        });

        // Called when you delete the video.
        uploader.bind('deleteComplete', function(event, fileId, response) {
          $('#' + id + '-value').val('');
        });

        // Called when the upload is completed.
        uploader.bind('complete', function(event, fileId, name, response) {
          if (response.success) {
            var file = $(this).fineUploaderS3("getItemByFileId", fileId);
            var viewBtn = file.find(".view-btn");
            viewBtn.show();
            viewBtn.attr("href", response.tempURL);

            // Set the value of this upload.
            $('#' + id + '-value').val(response.url);
          }
        });
      });
    }
  });
})(jQuery);
