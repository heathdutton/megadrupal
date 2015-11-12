/**
 * @file
 * Allow direct upload by drag and dropping local images into CKEditor.
 */

'use strict';

(function ($) {

  CKEDITOR.plugins.add('directupload', {
    requires: 'uploadwidget',

    onLoad: function () {
      CKEDITOR.addCss(
        '.cke_upload_uploading img{' +
        'opacity: 0.3' +
        '}'
      );
    },

    init: function (editor) {
      // Do not execute this paste lister if it will not be possible to upload file.
      if (!CKEDITOR.plugins.clipboard.isFileApiSupported) {
        return;
      }

      var fileTools = CKEDITOR.fileTools,
      //uploadUrl = fileTools.getUploadUrl( editor.config, 'image' );
        uploadUrl = Drupal.settings.basePath + 'direct_upload/upload';

      // Handle images which are available in the dataTransfer.
      fileTools.addUploadWidget(editor, 'uploadimage', {
        supportedTypes: /image\/(jpeg|png|gif)/,

        uploadUrl: uploadUrl,

        fileToElement: function () {
          var img = new CKEDITOR.dom.element('img');
          img.setAttribute('src', loadingImage);
          return img;
        },

        parts: {
          img: 'img'
        },

        onUploading: function (upload) {
          // Show the image during the upload.
          this.parts.img.setAttribute('src', upload.data);
        },

        onUploaded: function (upload) {
          Drupal.dnd.refreshLibraries();
          var sid = upload.url;
          var range = editor.createRange();
          range.setStartBefore(this.wrapper);
          range.setEndAfter(this.wrapper);

          // @todo improve this.
          // editor.dndInsertAtom does not work with fetchAtom() because it
          // requires atomInfo.sas etc., so we have to "wait" for the library to
          // be refreshed.
          var insertAtom = function () {
            if (Drupal.dnd.Atoms[sid]) {
              editor.dndInsertAtom(sid);
            }
            else {
              setTimeout(insertAtom, 200);
            }
          };
          insertAtom();
        }
      });

      // Handle images which are not available in the dataTransfer.
      // This means that we need to read them from the <img src="data:..."> elements.
      editor.on('paste', function (evt) {
        // For performance reason do not parse data if it does not contain img tag and data attribute.
        if (!evt.data.dataValue.match(/<img[\s\S]+data:/i)) {
          return;
        }

        var data = evt.data,
        // Prevent XSS attacks.
          tempDoc = document.implementation.createHTMLDocument(''),
          temp = new CKEDITOR.dom.element(tempDoc.body),
          imgs, img, i;

        // Without this isReadOnly will not works properly.
        temp.data('cke-editable', 1);
        temp.appendHtml(data.dataValue);

        imgs = temp.find('img');
        for (i = 0; i < imgs.count(); i++) {
          img = imgs.getItem(i);

          // Image have to contain src=data:...
          var isDataInSrc = img.getAttribute('src') && img.getAttribute('src').substring(0, 5) == 'data:';

          // We are not uploading images in non-editable blocs.
          if (isDataInSrc && !img.data('cke-upload-id') && !img.isReadOnly(1)) {
            var loader = editor.uploadsRepository.create(img.getAttribute('src'));
            loader.upload(uploadUrl);
            fileTools.markElement(img, 'uploadimage', loader.id);
            fileTools.bindNotifications(editor, loader);
          }
        }

        data.dataValue = temp.getHtml();
      });
    }
  });

  // Black rectangle which is shown before image is loaded.
  var loadingImage = 'data:image/gif;base64,R0lGODlhDgAOAIAAAAAAAP///yH5BAAAAAAALAAAAAAOAA4AAAIMhI+py+0Po5y02qsKADs=';

})(jQuery);
