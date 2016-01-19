/**
 * Contains CKEditor plugin logic.
 */

CKEDITOR.plugins.add('drupalimagefromword', {
  requires : ['dialog'],
  lang: 'ru,en',
  init: function (editor) {
    var lang = editor.lang.drupalimagefromword;

    editor.on('paste', function (ev) {
      // Check pasted content for images.
      tools.checkImages(ev);
    });

    // Some tools to import images into CKEditor.
    var tools = {
      checkImages: function(ev) {
        // Wrap text into div, to convert text into HTML and find images there.
        var images = jQuery('<div>' + ev.data.dataValue + '</div>').find('img');

        // Check all types of images.
        this.checkImagesLocal(ev, images);
        this.checkImagesBase64(ev, images);
        this.checkImagesBlob(ev);
      },
      // Check pasted value for images with src like "file:///some/path"
      checkImagesLocal: function(ev, images) {
        // Try to find images with file:/// protocol (local images).
        var localImagesCount = 0;
        jQuery.each(images, function(imgTagKey, imgTag) {
          var localImage = jQuery(imgTag).attr('src').match(/file:\/\/\//);
          if (localImage) {
            var filename = imgTag.src.split('/').pop();
            if (filename) {
              // Remove img with local path.
              var pattern = '<img.{1,}src=".{1,}' + filename + '".{0,}?>';
              var regex = new RegExp(pattern);
              var toReplace = ev.data.dataValue.match(regex);
              if (toReplace) {
                ev.data.dataValue = ev.data.dataValue.replace(toReplace[0], '');
                localImagesCount++;
              }
            }
          }
        });

        // Show notification about local images, which cannot be loaded due the security reasons.
        if (localImagesCount) {
          editor.config.drupalImageFromWord = {};
          editor.config.drupalImageFromWord.message = lang.error_text + '<br />' + lang.error_recommendation;
          dialogCommand.exec(ev.editor);
        }
      },
      // Check pasted value for images with base64 value in src.
      checkImagesBase64: function(ev, images) {
        var base64values = [];
        // Try to find images with base64 in src.
        jQuery.each(images, function(imgTagKey, imgTag) {
          var base64 = jQuery(imgTag).attr('src').match(/data:image\/[a-z]{1,4};base64.{1,}/)
          if (base64) {
            base64values[base64values.length] = base64[0];
          }
        });

        // If we have images with base64 in src, we should save them.
        if (base64values.length) {
          var fd = new FormData();

          var fdIsEmpty = true;

          jQuery.each(base64values, function(fileKey, imgValue) {
            // Convert base64 into blob.
            var byteString = atob(imgValue.split(',')[1]);
            var ab = new ArrayBuffer(byteString.length);
            var ia = new Uint8Array(ab);
            for (var i = 0; i < byteString.length; i++) {
              ia[i] = byteString.charCodeAt(i);
            }

            // Get mime type.
            var mimetype = imgValue.match(/data:image\/[a-z]{1,5}/);
            mimetype = mimetype[0] ? mimetype[0].replace('data:', '') : null;

            if (mimetype) {
              // Make file.
              var file = new Blob([ab], {type: mimetype});

              // Get file extension.
              var fileExtension = mimetype.replace('image/', '');

              // Add file into fd.
              if (fileExtension) {
                fd.append('files[ifw_' + fileKey + ']', file, 'filefromword' + fileKey +'.' + fileExtension);
                fdIsEmpty = false;
              }
            }
          });

          // If we have files in fd, then upload them.
          if (!fdIsEmpty) {
            this.uploadFiles(ev, fd, 'base64', base64values);
          }
        }
      },
      // Check pasted value for images, pasted as blob.
      checkImagesBlob: function(ev) {
        if (ev.data.dataTransfer._.files) {
          var fd = new FormData();
          jQuery.each(ev.data.dataTransfer._.files, function(fileKey, blob) {
            var fileExt = ev.data.dataTransfer._.files[fileKey].type.replace('image/', '');
            fd.append('files[ifw_' + fileKey + ']', blob, 'filefromword' + fileKey +'.' + fileExt);
          });
          this.uploadFiles(ev, fd, 'blob', []);
        }
      },
      // Upload files to server and show them in CKEditor.
      uploadFiles: function(ev, fd, mode, base64values) {
        jQuery.ajax({
          url: Drupal.settings.basePath + Drupal.settings.pathPrefix + 'image-from-word/import/files',
          method: 'POST',
          data: fd,
          processData: false,
          contentType: false,
          dataType: 'json',
          async: false,
          success: function (response) {
            if (response) {
              var errors = [];
              jQuery.each(response, function(imgTagKey, file) {
                if (file.url) {
                  // Replace base64 to normal url.
                  if (mode == 'base64') {
                    ev.data.dataValue = ev.data.dataValue.replace(base64values[imgTagKey], file.url);
                  }
                  // Paste image for blob mode.
                  else {
                    ev.data.dataValue = '<img src="' + file.url + '" />';
                  }
                }
                // Collect errors to show them later.
                else if (file.error) {
                  errors[errors.length] = file.error;
                }
              });

              // If we have errors, show them.
              if (errors.length) {
                editor.config.drupalImageFromWord = {};
                editor.config.drupalImageFromWord.errors = errors;
                dialogCommand.exec(ev.editor);
              }
            }
          }
        });
      }
    };
    // Init dialog.
    CKEDITOR.dialog.add('drupalimagefromwordDialog', this.path + 'dialogs/drupalimagefromword.js');
    var dialogCommand = new CKEDITOR.dialogCommand('drupalimagefromwordDialog');
  }
});